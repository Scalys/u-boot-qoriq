// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 * Copyright 2019 NXP
 */

#include <common.h>
#include <i2c.h>
#include <asm/io.h>
#include <asm/arch/clock.h>
#include <asm/arch/fsl_serdes.h>
#ifdef CONFIG_FSL_LS_PPA
#include <asm/arch/ppa.h>
#endif
#include <asm/arch/mmu.h>
#include <asm/arch/soc.h>
#include <hwconfig.h>
#include <ahci.h>
#include <mmc.h>
#include <scsi.h>
#include <fsl_esdhc.h>
#include <env_internal.h>
#include <fsl_mmdc.h>
#include <netdev.h>
#include <fsl_sec.h>

#include "gpio.h"
#include "usb_trustbox.h"
#include "board_configuration_data.h"

DECLARE_GLOBAL_DATA_PTR;

/* Possible SPI (NOR) flashes used as rescue memory device */
const char* supported_rescue_flashes[] = {"s25fs064s","sst26wf080b"};
static int recovery_mode_enabled = 0;

int checkboard(void)
{
#ifdef CONFIG_TARGET_TRUSTBOX
	struct ccsr_gpio *pgpio = (void *)(CONFIG_SYS_GPIO2);
	const void* bcd_dtc_blob;
	int ret;

	int m2_config = 0;
	int serdes_cfg = get_serdes_protocol();

	puts("Board: Trustbox\n");

	env_set_ulong("recoverymode", recovery_mode_enabled);
	bcd_dtc_blob = get_boardinfo_rescue_flash();
	if (bcd_dtc_blob != NULL) {
		/* Board Configuration Data is intact, ready for parsing */
		ret = add_mac_addressess_to_env(bcd_dtc_blob);
		if (ret != 0) {
			printf("Error adding BCD data to environment\n");
		}
	}

	/* set QSPI chip select muxing to 0 */
	setbits_be32(&pgpio->gpdir, QSPI_MUX_N_MASK);
	clrbits_be32(&pgpio->gpdat, QSPI_MUX_N_MASK);

	/* Check for an M.2 module */
	clrbits_be32(&pgpio->gpdir, (M2_CFG0 | M2_CFG1 | M2_CFG2 | M2_CFG3));
	m2_config = (in_be32(&pgpio->gpdat) & (M2_CFG0 | M2_CFG1 | M2_CFG2 | M2_CFG3));

	switch(m2_config) {
	case 0:
		printf("M.2: SATA SSD module found\n");
		if (serdes_cfg & 0x5)
			printf("Warning: SERDES has not been configured in RCW for SATA!\n");
		break;
	case M2_CFG1:
		printf("M.2: PCIe SSD module found\n");
		if (serdes_cfg & 0x8)
			printf("Warning: SERDES has not been configured in RCW for PCIe!\n");
		break;
	case (M2_CFG0 | M2_CFG1 | M2_CFG2 | M2_CFG3):
		printf("M.2: No module detected\n");
		break;
	default:
		printf("M.2: A module has been detected (TODO: support new module type)\n");
		break;
	}


	/* Configure USB hub */
	usb_hx3_hub_init();

#else
	puts("Board: unknown");
#endif
	return 0;
}

#ifdef CONFIG_TFABOOT
int dram_init(void)
{
	gd->ram_size = tfa_get_dram_size();
	if (!gd->ram_size)
		gd->ram_size = CONFIG_SYS_SDRAM_SIZE;

	return 0;
}
#else
int dram_init(void)
{
#ifndef CONFIG_TFABOOT
	static const struct fsl_mmdc_info mparam = {
		0x05180000,	/* mdctl */
		0x00030035,	/* mdpdc */
		0x12554000,	/* mdotc */
		0xbabf7954,	/* mdcfg0 */
		0xdb328f64,	/* mdcfg1 */
		0x01ff00db,	/* mdcfg2 */
		0x00001680,	/* mdmisc */
		0x0f3c8000,	/* mdref */
		0x00002000,	/* mdrwd */
		0x00bf1023,	/* mdor */
		0x0000003f,	/* mdasp */
		0x0000022a,	/* mpodtctrl */
		0xa1390003,	/* mpzqhwctrl */
	};

	mmdc_init(&mparam);
#endif

	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
#if !defined(CONFIG_SPL) || defined(CONFIG_SPL_BUILD)
	/* This will break-before-make MMU for DDR */
	update_early_mmu_table();
#endif

	return 0;
}
#endif


int board_early_init_f(void)
{
	fsl_lsch2_early_init_f();

	return 0;
}

int board_init(void)
{
	struct ccsr_cci400 *cci = (struct ccsr_cci400 *)(CONFIG_SYS_IMMR +
					CONFIG_SYS_CCI400_OFFSET);
	struct ccsr_gpio *pgpio = (void *)(CONFIG_SYS_GPIO2);
	/*
	 * Set CCI-400 control override register to enable barrier
	 * transaction
	 */
	if (current_el() == 3)
		out_le32(&cci->ctrl_ord, CCI400_CTRLORD_EN_BARRIER);

#ifdef CONFIG_SYS_FSL_ERRATUM_A010315
	erratum_a010315();
#endif

#ifdef CONFIG_ENV_IS_NOWHERE
	gd->env_addr = (ulong)&default_environment[0];
#endif

	/* Detect and handle grapeboard rescue mode */
	char *current_flash_name = strdup(get_qspi_flash_name());
	for (int index = 0; index < sizeof(supported_rescue_flashes) / sizeof(supported_rescue_flashes[0]); index++) {
		if(strcmp(current_flash_name, supported_rescue_flashes[index]) == 0) {
			/* Revert chip select muxing to standard QSPI flash */
			setbits_be32(&pgpio->gpdir, QSPI_MUX_N_MASK);
			clrbits_be32(&pgpio->gpdat, QSPI_MUX_N_MASK);
			printf("Please release the rescue mode button (S2) to enter the recovery mode\n");
			recovery_mode_enabled = 1;

			while(strcmp(get_qspi_flash_name(), supported_rescue_flashes[index]) == 0) {
				udelay(500000);
				puts("\033[1A"); /* Overwrite previous line */
			}
		}
	}
	free(current_flash_name);

#ifdef CONFIG_FSL_CAAM
	sec_init();
#endif

#ifdef CONFIG_FSL_LS_PPA
	ppa_init();
#endif

	return 0;
}

#ifdef CONFIG_TARGET_TRUSTBOX
int esdhc_status_fixup(void *blob, const char *compat)
{
	char esdhc0_path[] = "/soc/esdhc@1560000";

	do_fixup_by_path(blob, esdhc0_path, "status", "okay",
			 sizeof("okay"), 1);

	return 0;
}
#endif

int ft_board_setup(void *blob, bd_t *bd)
{
	arch_fixup_fdt(blob);

	ft_cpu_setup(blob, bd);

	return 0;
}
