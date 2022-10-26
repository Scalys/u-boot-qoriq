/*
 * Copyright 2016 Freescale Semiconductor, Inc.
 *           2021 Scalys
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <netdev.h>
#include <fm_eth.h>
#include <fsl_dtsec.h>
#include <fsl_mdio.h>
#include <malloc.h>
#include "../../freescale/common/fman.h"
#include "dragonfruit.h"
#include <i2c.h>

static int scalys_carrier_init_rgmii_phy0(int enable);
static int scalys_carrier_init_rgmii_phy1(int enable);
static int scalys_carrier_init_sfp_phys(void);
static int scalys_carrier_init_retimer(int enable);

/* Tested module(s):
 * FCLF-8522, UF-RJ45-1G, JD089BST
 */
uint8_t sfp_phy_config[][2] = {
	{ 0x1b, 0x90 }, /* Enable SGMII mode */
	{ 0x1b, 0x84 },
	{ 0x09, 0x0F }, /* Advertise 1000BASE-T H/Full */
	{ 0x09, 0x00 },
	{ 0x00, 0x81 }, /* Apply software reset */
	{ 0x00, 0x40 },
	{ 0x04, 0x0D }, /* Advertise 100/10BASE-T H/Full */
	{ 0x04, 0xE1 },
	{ 0x00, 0x91 }, /* Apply software reset */
	{ 0x00, 0x40 },
};

#define SFP_PHY_CONFIG_ITEM_NBR (sizeof(sfp_phy_config) / 2)

static int scalys_carrier_init_rgmii_phy0(int enable)
{
	int ret;
	u32 val;
	struct ccsr_gpio *pgpio2 = (void *)(GPIO2_BASE_ADDR);

	 /* Reset RGMII PHY 0 by setting ETH1_RESET_N_3V3 high */
	val = in_be32(&pgpio2->gpdir) | ETH1_RESET_N_3V3_GPIO;
	out_be32(&pgpio2->gpdir, val);

	val = in_be32(&pgpio2->gpdat);
	val &= ~ETH1_RESET_N_3V3_GPIO;
	out_be32(&pgpio2->gpdat, val);

	/* Wait for 10 ms to to meet reset timing */
	mdelay(10);

	if (enable) {
		val = in_be32(&pgpio2->gpdat) | ETH1_RESET_N_3V3_GPIO;
		out_be32(&pgpio2->gpdat, val);

		/* Write 0x4111 to reg 0x18 on both PHYs to change LEDs usage */
		ret = miiphy_write("FSL_MDIO0",0,0x18,0x4111);
	}

	return ret;
}

static int scalys_carrier_init_rgmii_phy1(int enable)
{
	int ret = 0;
	uint8_t i2c_data;

	/* Reset RGMII PHY 1 by setting ETH2_RESET_N_3V3 high */
	i2c_set_bus_num(0);
	/* Set to output */
	ret = i2c_read(MOD_I2C_IO_ADDR, 0x0E, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	i2c_data &= ~ETH2_RESET_N_3V3_GPIO;
	ret = i2c_write(MOD_I2C_IO_ADDR, 0x0E, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	/* Set to low */
	ret = i2c_read(MOD_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	i2c_data &= ~ETH2_RESET_N_3V3_GPIO;
	ret = i2c_write(MOD_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}

	/* Wait for 10 ms to to meet reset timing */
	mdelay(10);

	if (enable) {
		/* Set to high */
		ret = i2c_read(MOD_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
		if (ret) {
			return ret;
		}
		i2c_data |= ETH2_RESET_N_3V3_GPIO;
		ret = i2c_write(MOD_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
		if (ret) {
			return ret;
		}

		/* Write 0x4111 to reg 0x18 on both PHYs to change LEDs usage */
		miiphy_write("FSL_MDIO0",1,0x18,0x4111);
	}

	return ret;
}

static int scalys_carrier_init_sfp_phys()
{
	int ret = 0;
	uint8_t i2c_data;

	/* Turn SFPx LEDs off by default */
	/* It is up to the OS to control these if desired */
	i2c_set_bus_num(0);
	ret = i2c_read(MOD_I2C_IO_ADDR, 0x0E, 1, &i2c_data, 1);
	i2c_data &= ~(SGMII0_LINKSTAT_GPIO| \
					SGMII0_ACT_GPIO| \
					SGMII1_LINKSTAT_GPIO| \
					SGMII1_ACT_GPIO);
	ret = i2c_write(MOD_I2C_IO_ADDR, 0x0E, 1, &i2c_data, 1);
	ret = i2c_read(MOD_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
	i2c_data &= ~(SGMII0_LINKSTAT_GPIO| \
					SGMII0_ACT_GPIO| \
					SGMII1_LINKSTAT_GPIO| \
					SGMII1_ACT_GPIO);
	ret = i2c_write(MOD_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);

	/* Remove SFP TX_disable */
	/* Set to output */
	ret = i2c_read(CAR_I2C_IO_ADDR, 0x0E, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	i2c_data &= ~TX_DISABLE_GPIO;
	ret = i2c_write(CAR_I2C_IO_ADDR, 0x0E, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	/* Set to low */
	ret = i2c_read(CAR_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	i2c_data &= ~TX_DISABLE_GPIO;
	ret = i2c_write(CAR_I2C_IO_ADDR, 0x06, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}

	/* Wait until PHYs are ready */
	mdelay(100);

	i2c_set_bus_num(3);

	/* If inserted, the SFP modules have to be configured. */
	for (int phy_addr=2; phy_addr<4; phy_addr++) {
		/* I2C multiplexer channel selection (SGMII/SFP is 2 (bottom left) & 3 (top left)) */
		i2c_data = (1 << phy_addr);
		ret = i2c_write(0x70, 0, 1, &i2c_data, 1);
		if (ret) {
			printf("Error Setting SFP i2c MUX\n");
			break;
		}

		ret = i2c_probe(0x56);
		if (ret) {
			printf("Warning: No SFP PHY module detected on slot %i.\n", phy_addr);
		} else {
			for (int i = 0; i < SFP_PHY_CONFIG_ITEM_NBR; i++) {
				ret = i2c_write(0x56, sfp_phy_config[i][0], 1, &(sfp_phy_config[i][1]), 1);
				if (ret) {
					printf("Error configuring SFP PHY on slot %i. (I2C Data: %d, Reg: %d).\n", phy_addr, sfp_phy_config[i][1], sfp_phy_config[i][0]);
					break;
				}
			}
		}
	}

	/* If SFP+ modules must be configured then add this below here */

	return 0;
}

static int scalys_carrier_init_retimer(int enable)
{
	int ret;
	uint8_t i2c_data;

	/* Set EDC_RST_N_3V3 low to keep retimer in reset */
	i2c_set_bus_num(0);
	/* Set to output */
	ret = i2c_read(MOD_I2C_IO_ADDR, 0x0D, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	i2c_data &= ~(EDC_RST_N_3V3_GPIO);
	ret = i2c_write(MOD_I2C_IO_ADDR, 0x0D, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}

	/* Set to low to enable PHY reset */
	ret = i2c_read(MOD_I2C_IO_ADDR, 0x05, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}
	i2c_data &= ~(EDC_RST_N_3V3_GPIO);
	ret = i2c_write(MOD_I2C_IO_ADDR, 0x05, 1, &i2c_data, 1);
	if (ret) {
		return ret;
	}

	/* Wait until PHYs are ready */
	mdelay(1);

	if (enable) {
		/* Set to high to disable PHY reset */
		ret = i2c_read(MOD_I2C_IO_ADDR, 0x05, 1, &i2c_data, 1);
		if (ret) {
			return ret;
		}
		i2c_data |= (EDC_RST_N_3V3_GPIO);
		ret = i2c_write(MOD_I2C_IO_ADDR, 0x05, 1, &i2c_data, 1);
		if (ret) {
			return ret;
		}

		mdelay(1);
	}

	return 0;
}

int board_eth_init(bd_t *bis)
{
#ifdef CONFIG_FMAN_ENET
	int i;
	struct memac_mdio_info dtsec_mdio_info;
	struct mii_dev *dev;
	u32 srds_s1;
	struct ccsr_gur *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR);

	srds_s1 = in_be32(&gur->rcwsr[4]) &	FSL_CHASSIS2_RCWSR4_SRDS1_PRTCL_MASK;
	srds_s1 >>= FSL_CHASSIS2_RCWSR4_SRDS1_PRTCL_SHIFT;

	dtsec_mdio_info.regs = (struct memac_mdio_controller *)CONFIG_SYS_FM1_DTSEC_MDIO_ADDR;
	dtsec_mdio_info.name = DEFAULT_FM_MDIO_NAME;

	/* Register the MDIO 1 bus */
	fm_memac_mdio_init(bis, &dtsec_mdio_info);

	/* Reset PHY's and do some initial mdio configuration */
	if(scalys_carrier_init_rgmii_phy0(true) != 0)
		printf("Initializing RGMII PHY 0 failed\n");
	if(scalys_carrier_init_rgmii_phy1(true) != 0)
		printf("Initializing RGMII PHY 1 failed\n");
	if(scalys_carrier_init_sfp_phys() != 0)
		printf("Initializing SFP PHYs failed\n");

	/* Wait until PHYs are ready */
	mdelay(100);

	/* Set the two RGMII PHY addresses */
	fm_info_set_phy_address(FM1_DTSEC3, RGMII_PHY1_ADDR);
	fm_info_set_phy_address(FM1_DTSEC4, RGMII_PHY2_ADDR);

	/* The SGMII PHYs have no MDIO interface and have already been configured
	 * above through I2C */
	/* Disable unused DTSEC ports */
	fm_disable_port(FM1_DTSEC1);
	fm_disable_port(FM1_DTSEC2);
	fm_disable_port(FM1_DTSEC9);
	fm_disable_port(FM1_DTSEC10);

	dev = miiphy_get_dev_by_name(DEFAULT_FM_MDIO_NAME);
	for (i = FM1_DTSEC1; i < FM1_DTSEC1 + CONFIG_SYS_NUM_FM1_DTSEC; i++) {
		/* Skip SGMII interfaces since their PHYs are not connected to the MDIO bus */
		if (fm_info_get_enet_if(i) == PHY_INTERFACE_MODE_SGMII) {
			continue;
		}
		fm_info_set_mdio(i, dev);
	}

	/* Only support XFI interfaces if correct SERDES1 configuration is present */
	/* Note: due to design limitation the MDIO 1 bus will be used for the CS4315 retimer PHYs as well.
	 * This retimer expects clause 45 formatted data but the RGMII PHYs expect clause 22.
	 * The RGMII PHYs can become confused by the clause 45 data and may incorrectly respond
	 * under certain circumstances. For now the retimer PHYs have higher addresses to hopefully
	 * bypass this but this may still result in undesired behaviour */
	if (srds_s1 == 0x1133) {
		/* First remove reset */
		if(scalys_carrier_init_retimer(true) != 0) {
			printf("Enabling XFI retimer failed\n");
		}

		/* Set the two XFI PHY addresses and associate them with the MDIO bus */
		fm_info_set_phy_address(FM1_10GEC1, CORTINA_PHY_ADDR1);
		fm_info_set_phy_address(FM1_10GEC2, CORTINA_PHY_ADDR2);
		dev = miiphy_get_dev_by_name(DEFAULT_FM_MDIO_NAME);
		fm_info_set_mdio(FM1_10GEC1, dev);
		fm_info_set_mdio(FM1_10GEC2, dev);
	} else {
		/* Disable the retimer by keeping it in reset */
		if(scalys_carrier_init_retimer(false) != 0) {
			printf("Disabling XFI retimer failed\n");
		}
		fm_disable_port(FM1_10GEC1);
		fm_disable_port(FM1_10GEC2);
	}

	cpu_eth_init(bis);
#endif

	return pci_eth_init(bis);
}
