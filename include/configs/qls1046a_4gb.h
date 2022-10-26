/*
 * Copyright 2019 Teledyne e2v
 * Copyright 2020 Scalys
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __QLS1046A_4GB_H__
#define __QLS1046A_4GB_H__

#include "ls1046a_common.h"

#define CONFIG_SYS_CLK_FREQ		100000000
#define CONFIG_DDR_CLK_FREQ		100000000

#define CONFIG_LAYERSCAPE_NS_ACCESS

#ifndef CONFIG_MISC_INIT_R
#define CONFIG_MISC_INIT_R
#endif

#ifndef CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_BOARD_INIT
#endif

/* Card detect pin is not connected properly so read out gpio4_3 instead */
#define CONFIG_ESDHC_DETECT_QUIRK \
	((in_be32(GPIO4_BASE_ADDR + 0x8) & 0x10000000) ? 0 : 1)

#define CONFIG_DIMM_SLOTS_PER_CTLR	1
/* Physical Memory Map */
#define CONFIG_CHIP_SELECTS_PER_CTRL	4

#ifdef CONFIG_DDR_SPD
#undef CONFIG_DDR_SPD
#endif

#define CONFIG_SYS_DDR_RAW_TIMING
#define CONFIG_DDR_ECC
#define CONFIG_ECC_INIT_VIA_DDRCONTROLLER
#define CONFIG_MEM_INIT_VALUE           0xdeadbeef

/* Size of malloc() pool */
#ifdef CONFIG_SYS_MALLOC_LEN
#undef CONFIG_SYS_MALLOC_LEN
#endif
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + 32 * 1024 * 1024)

#ifdef CONFIG_RAMBOOT_PBL
#define CONFIG_SYS_FSL_PBL_PBI board/scalys/qls1046a_4gb/qls1046a_4gb_pbi.cfg
#endif

#ifdef CONFIG_SD_BOOT
#ifdef CONFIG_PHY_CORTINA
#define CONFIG_SYS_FSL_PBL_RCW board/scalys/qls1046a_4gb/qls1046a_4gb_rcw_sd_xfi.cfg
#else /* CONFIG_PHY_CORTINA */
#define CONFIG_SYS_FSL_PBL_RCW board/scalys/qls1046a_4gb/qls1046a_4gb_rcw_sd.cfg
#endif /* CONFIG_PHY_CORTINA */
#elif defined(CONFIG_NOR_BOOT)
#ifdef CONFIG_PHY_CORTINA
#define CONFIG_SYS_FSL_PBL_RCW board/scalys/qls1046a_4gb/qls1046a_4gb_rcw_ifc_nor_xfi.cfg
#else /* CONFIG_PHY_CORTINA */
#define CONFIG_SYS_FSL_PBL_RCW board/scalys/qls1046a_4gb/qls1046a_4gb_rcw_ifc_nor.cfg
#endif /* CONFIG_PHY_CORTINA */
#elif defined(CONFIG_QSPI_BOOT) || defined(CONFIG_SD_BOOT_QSPI)
#error QSPI interface is not supported
#elif defined(CONFIG_EMMC_BOOT)
#error EMMC boot not supported
#else
#error Unsupported boot source
#endif

/* IFC */
#define CONFIG_FSL_IFC

/* NAND Flash on IFC CS0 */
#define CONFIG_NAND_FSL_IFC
#define CONFIG_SYS_NAND_BASE		0x7e800000
#define CONFIG_SYS_NAND_BASE_PHYS	CONFIG_SYS_NAND_BASE

#define CONFIG_SYS_NAND_CSPR_EXT        (0x0)
#define CONFIG_SYS_NAND_CSPR            (CSPR_PHYS_ADDR(CONFIG_SYS_NAND_BASE_PHYS) \
                                        | CSPR_PORT_SIZE_8 /* Port Size = 8 bit */ \
                                        | CSPR_MSEL_NAND    /* MSEL = NAND */ \
                                        | CSPR_V)

#define CONFIG_SYS_NAND_AMASK           IFC_AMASK(64*1024)

#define CONFIG_SYS_NAND_CSOR		(CSOR_NAND_ECC_ENC_EN |	/* ECC on encode */ \
					 CSOR_NAND_ECC_DEC_EN |	/* ECC on decode */ \
					 CSOR_NAND_ECC_MODE_4 |	/* 4-bit ECC */ \
					 CSOR_NAND_RAL_3 |	/* RAL = 3Byes */ \
					 CSOR_NAND_PGS_2K |	/* Page Size = 2K */ \
					 CSOR_NAND_SPRZ_128 |	/* Spare size = 128 */ \
					 CSOR_NAND_PB(64) |	/* Pages Per Block = 64 */ \
					 CSOR_NAND_BCTLD)	/* Buffer control disable */

#define CONFIG_SYS_NAND_ONFI_DETECTION

#define CONFIG_SYS_NAND_FTIM0           (FTIM0_NAND_TCCST(0x07) | \
                                        FTIM0_NAND_TWP(0x18)   | \
                                        FTIM0_NAND_TWCHT(0x07) | \
                                        FTIM0_NAND_TWH(0x0a))
#define CONFIG_SYS_NAND_FTIM1           (FTIM1_NAND_TADLE(0x32) | \
                                        FTIM1_NAND_TWBE(0x39)  | \
                                        FTIM1_NAND_TRR(0x0e)   | \
                                        FTIM1_NAND_TRP(0x18))
#define CONFIG_SYS_NAND_FTIM2           (FTIM2_NAND_TRAD(0x0f) | \
                                        FTIM2_NAND_TREH(0x0a) | \
                                        FTIM2_NAND_TWHRE(0x1e))
#define CONFIG_SYS_NAND_FTIM3           FTIM3_NAND_TWW(0xa)

#define CONFIG_SYS_NAND_BASE_LIST       { CONFIG_SYS_NAND_BASE }
#define CONFIG_SYS_MAX_NAND_DEVICE      1
#define CONFIG_MTD_NAND_VERIFY_WRITE

#define CONFIG_SYS_NAND_BLOCK_SIZE	(128 * 1024)

/* NOR Flash on IFC CS1 */
#define CONFIG_SYS_FLASH_CFI_WIDTH	FLASH_CFI_16BIT
#define CONFIG_SYS_FLASH_PROTECTION

#define CONFIG_SYS_FLASH_BASE			0x60000000
#define CONFIG_SYS_FLASH_BASE_PHYS		CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_FLASH_BASE_PHYS_EARLY	0x00000000

#define CONFIG_SYS_MAX_FLASH_BANKS	1
#define CONFIG_SYS_MAX_FLASH_SECT	259

#define CONFIG_SYS_FLASH_EMPTY_INFO
#define CONFIG_SYS_FLASH_BANKS_LIST	{CONFIG_SYS_FLASH_BASE_PHYS}

#define CONFIG_SYS_NOR_CSPR_EXT		(0x0)
#define CONFIG_SYS_NOR_CSPR            (CSPR_PHYS_ADDR(CONFIG_SYS_FLASH_BASE_PHYS) | \
					CSPR_PORT_SIZE_16 | \
					CSPR_MSEL_NOR | \
					CSPR_V)

#define CONFIG_SYS_NOR_AMASK		IFC_AMASK(32*1024*1024)

#define CONFIG_SYS_NOR_CSOR \
    (CSOR_NOR_AVD_TGL_PGM_EN | /* AVD Toggle Enable during Burst Program */ \
     CSOR_NOR_ADM_SHIFT(7) | /* Address Data Multiplexing Shift */ \
     CSOR_NOR_TRHZ_80) | /* Time for Read Enable High to Output High Impedance */ \
     CSOR_NAND_BCTLD /* Buffer control disable */

#define CONFIG_SYS_NOR_FTIM0	(FTIM0_NOR_TACSE(0x4) | \
				FTIM0_NOR_TEADC(0x5) | \
				FTIM0_NOR_TEAHC(0x5))
#define CONFIG_SYS_NOR_FTIM1	(FTIM1_NOR_TACO(0x35) | \
				FTIM1_NOR_TRAD_NOR(0x1A) |\
				FTIM1_NOR_TSEQRAD_NOR(0x13))
#define CONFIG_SYS_NOR_FTIM2	(FTIM2_NOR_TCS(0x4) | \
				FTIM2_NOR_TCH(0x4) | \
				FTIM2_NOR_TWPH(0x0E) | \
				FTIM2_NOR_TWP(0x1c))
#define CONFIG_SYS_NOR_FTIM3	0x0

#define CONFIG_CFI_FLASH_USE_WEAK_ACCESSORS

/* IFC Timing Params (CONFIG_NAND_BOOT not supported!) */
#if defined(CONFIG_NOR_BOOT) || defined (CONFIG_SD_BOOT)
/* CS0 */
#define CONFIG_SYS_CSPR0_EXT		CONFIG_SYS_NOR_CSPR_EXT
#define CONFIG_SYS_CSPR0		CONFIG_SYS_NOR_CSPR
#define CONFIG_SYS_AMASK0		CONFIG_SYS_NOR_AMASK
#define CONFIG_SYS_CSOR0		CONFIG_SYS_NOR_CSOR
#define CONFIG_SYS_CS0_FTIM0		CONFIG_SYS_NOR_FTIM0
#define CONFIG_SYS_CS0_FTIM1		CONFIG_SYS_NOR_FTIM1
#define CONFIG_SYS_CS0_FTIM2		CONFIG_SYS_NOR_FTIM2
#define CONFIG_SYS_CS0_FTIM3		CONFIG_SYS_NOR_FTIM3
/* CS1 */
#define CONFIG_SYS_CSPR1_EXT		CONFIG_SYS_NAND_CSPR_EXT
#define CONFIG_SYS_CSPR1		CONFIG_SYS_NAND_CSPR
#define CONFIG_SYS_AMASK1		CONFIG_SYS_NAND_AMASK
#define CONFIG_SYS_CSOR1		CONFIG_SYS_NAND_CSOR
#define CONFIG_SYS_CS1_FTIM0		CONFIG_SYS_NAND_FTIM0
#define CONFIG_SYS_CS1_FTIM1		CONFIG_SYS_NAND_FTIM1
#define CONFIG_SYS_CS1_FTIM2		CONFIG_SYS_NAND_FTIM2
#define CONFIG_SYS_CS1_FTIM3		CONFIG_SYS_NAND_FTIM3
#endif /* CONFIG_NOR_BOOT */

/* Environment */
#ifndef SPL_NO_ENV
#define CONFIG_ENV_OVERWRITE
#endif

/* Boot specific env and firmware configuration */
#if defined(CONFIG_SD_BOOT)
#define CONFIG_SYS_MMC_ENV_DEV		0
#define CONFIG_ENV_OFFSET		(3 * 1024 * 1024)
#define CONFIG_ENV_SIZE			0x2000
/*Added for Cortina support */
#define CONFIG_SYS_CORTINA_FW_IN_NOR
#define CONFIG_CORTINA_FW_ADDR		0x60280000
#elif defined(CONFIG_NOR_BOOT)
#define CONFIG_ENV_SIZE			0x2000		/* 8KB */
#define CONFIG_ENV_OFFSET		0x200000	/* 2MB */
#define CONFIG_ENV_SECT_SIZE		0x40000		/* 256KB */
/*Added for Cortina support */
#define CONFIG_SYS_CORTINA_FW_IN_NOR
#define CONFIG_CORTINA_FW_ADDR		0x60280000
#ifdef CONFIG_SYS_FMAN_FW_ADDR
#undef CONFIG_SYS_FMAN_FW_ADDR /* already defined in ls1046a_common.h */
#endif /* CONFIG_SYS_FMAN_FW_ADDR */
#define CONFIG_SYS_FMAN_FW_ADDR		(CONFIG_SYS_FLASH_BASE + 0x240000)
#else
#define CONFIG_ENV_SIZE			0x2000		/* 8KB */
#define CONFIG_ENV_OFFSET		0x300000	/* 3MB */
#define CONFIG_ENV_SECT_SIZE		0x40000		/* 256KB */
#endif

/* NOR boot SPL */
#ifdef CONFIG_NOR_BOOT
#define CONFIG_SPL_TARGET		"spl/u-boot-spl.pbl"
#define CONFIG_SPL_TEXT_BASE		0x10000000
#define CONFIG_SPL_MAX_SIZE		0x1f000
#define CONFIG_SPL_STACK		0x10020000
#define CONFIG_SPL_PAD_TO		0x20000
#define CONFIG_SPL_BSS_START_ADDR	0x8f000000
#define CONFIG_SPL_BSS_MAX_SIZE		0x80000
#define CONFIG_SYS_SPL_MALLOC_START	(CONFIG_SPL_BSS_START_ADDR + \
					CONFIG_SPL_BSS_MAX_SIZE)
#define CONFIG_SYS_SPL_MALLOC_SIZE	0x100000
#define CONFIG_SYS_MONITOR_LEN		0x100000
#define CONFIG_SYS_UBOOT_BASE		0x60020000 /* CONFIG_SYS_FLASH_BASE + CONFIG_SPL_PAD_TO */
#define CONFIG_SYS_UBOOT_START		0x82000000
#ifdef SPL_NO_IFC
#undef SPL_NO_IFC
#endif
#endif

/* FMan */
#ifndef SPL_NO_FMAN

#ifdef CONFIG_NET
#define CONFIG_PHY_MARVELL
#ifdef CONFIG_SYS_DPAA_FMAN
#define CONFIG_FMAN_ENET

#define CONFIG_MII          /* MII PHY management */
#define CONFIG_ETHPRIME     "FM1@DTSEC3"
/* Change RGMII_TXID to RGMII mode in drivers/net/fm/ls1046.c */
#define LS1046A_FM1_DTSEC3_4_INTERFACE_MODE_RGMII
#define CONFIG_FMAN_ENET
#define RGMII_PHY1_ADDR			0x0
#define RGMII_PHY2_ADDR			0x1

/*Added for Cortina support*/
#ifdef CONFIG_PHY_CORTINA
#define CONFIG_CORTINA_FW_LENGTH	0x40000
#endif
#define CORTINA_PHY_ADDR1 		0xa
#define CORTINA_PHY_ADDR2		0xb

#endif  /* CONFIG_SYS_DPAA_FMAN */
#endif  /* CONFIG_NET */
#endif  /* !SPL_NO_FMAN */

/* USB */
#ifndef SPL_NO_USB
#ifdef CONFIG_HAS_FSL_XHCI_USB
#define CONFIG_USB_XHCI_HCD
#define CONFIG_USB_XHCI_FSL
#define CONFIG_USB_XHCI_DWC3
#define CONFIG_USB_MAX_CONTROLLER_COUNT         2
#endif /* CONFIG_HAS_FSL_XHCI_USB */
#endif /* !SPL_NO_USB */

/* Misc */
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

/* Dynamic MTD Partition support with mtdparts */
#define CONFIG_MTD
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_PARTITIONS
#define CONFIG_MTD_DEVICE
#define CONFIG_FLASH_CFI_MTD

/* UBI FS */
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_MTD_UBI
/*#define CONFIG_UBI_SILENCE_MSG*/
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_MTD_UBI_WL_THRESHOLD	4096
#define CONFIG_MTD_UBI_BEB_LIMIT	20

#define MTDIDS_DEFAULT CONFIG_MTDIDS_DEFAULT

/*Added for Cortina support (the cortina_ucode line)*/
#define MTDPARTS_DEFAULT    \
	"mtdparts="\
		"7e800000.flash:" \
			"2M@0x0(u-boot)," \
			"256k(env)," \
			"256k(fman_ucode)," \
			"256k(cortina_ucode)," \
			"0x3fc80000(ubipart_nand)," \
			"1M@0x3ff00000(bbt)ro;" \
		"60000000.nor:" \
			"2M@0x0(u-boot)," \
			"256k(env)," \
			"256k(fman_ucode)," \
			"256k(cortina_ucode)," \
			"-(ubipart_nor)"

#ifndef SPL_NO_MISC
#undef CONFIG_BOOTCOMMAND

#if defined(CONFIG_SD_BOOT)
#define BOOTCMD "run ubiboot-nand;"
#elif defined(CONFIG_NOR_BOOT)
#define BOOTCMD "run ubiboot-nand;"
#else
#define BOOTCMD ""
#endif
#define CONFIG_BOOTCOMMAND "run setfans;" BOOTCMD
#endif

#define SD_ENV	\
	"update-uboot-sd-nw=" \
		"dhcp; tftp ${load_addr} ${TFTP_PATH}/u-boot-with-spl-pbl.bin.sdhc;" \
		"if test $? = \"0\"; then " \
			"mmc rescan;" \
			"setexpr cnt ${filesize} + 0x1ff;" \
			"setexpr cnt ${cnt} / 0x200;" \
			"mmc write ${load_addr} 8 ${cnt};" \
		"fi\0" \
	"update-uboot-sd-usb=" \
		"usb start;" \
		"fatload usb 0 ${load_addr} u-boot-with-spl-pbl.bin.sdhc;" \
		"if test $? = \"0\"; then " \
			"mmc rescan;" \
			"setexpr cnt ${filesize} + 0x1ff;" \
			"setexpr cnt ${cnt} / 0x200;" \
			"mmc write ${load_addr} 8 ${cnt};" \
		"fi\0" \
	\
	"update-fman-ucode-sd-usb=" \
		"usb start;" \
		"fatload usb 0 ${load_addr} fsl_fman_ucode_ls1046_r1.0_106_4_18.bin;" \
		"if test $? = \"0\"; then " \
			"mmc rescan;" \
			"setexpr cnt ${filesize} + 0x1ff;" \
			"setexpr cnt ${cnt} / 0x200;" \
			"mmc write ${load_addr} 4800 ${cnt};" \
		"fi\0"

#define NOR_ENV	\
	"update-uboot-nor-nw=" \
		"dhcp; tftp ${load_addr} ${TFTP_PATH}/u-boot-with-spl-pbl.bin.nor;" \
		"if test $? = \"0\"; then " \
			"protect off nor0,0;" \
			"erase nor0,0;" \
			"cp.w ${load_addr} 0x60000000 ${filesize};" \
		"fi\0" \
	"update-uboot-nor-usb=" \
		"usb start;" \
		"fatload usb 0 ${load_addr} u-boot-with-spl-pbl.bin.nor;" \
		"if test $? = \"0\"; then " \
			"protect off nor0,0;" \
			"erase nor0,0;" \
			"cp.w ${load_addr} 0x60000000 ${filesize};" \
		"fi\0" \
	\
	"update-fman-ucode-nor-usb=" \
		"usb start;" \
		"fatload usb 0 ${load_addr} fsl_fman_ucode_ls1046_r1.0_106_4_18.bin;" \
		"if test $? = \"0\"; then " \
			"protect off nor0,2;" \
			"erase nor0,2;" \
			"cp.w ${load_addr} 0x60240000 ${filesize};" \
		"fi\0" \
	\
	"update-cortina-ucode-nor-usb=" \
		"usb start;" \
		"fatload usb 0 ${load_addr} cs4315-cs4340-PHY-ucode.txt;" \
		"if test $? = \"0\"; then " \
			"protect off nor0,3;" \
        	"erase nor0,3;" \
        	"cp.w ${load_addr} 0x60280000 ${filesize};" \
        "fi\0" \
    \
	"update-ubi-rootfs-nor="\
		"dhcp;" \
		"ubi part ubipart_nor;" \
		"if test $? = \"0\"; then " \
			"tftp ${load_addr} ${TFTP_PATH}/ubi_rootfs_image.nor.ubifs;" \
			"if test $? = \"0\"; then " \
				"ubi write ${load_addr} rootfs ${filesize};" \
			"fi;" \
		"fi;" \
		"\0" \
	\
	"ubiboot-nor=" \
		"ubi part ubipart_nor;" \
		"ubifsmount ubi0:rootfs;" \
		"ubifsload ${load_addr} /boot/fitImage;" \
		"run set_ubiboot_args_nor;" \
		"bootm ${load_addr}#conf@freescale_qls1046a-4gb-sdk.dtb" \
		"\0" \
	\
	"set_ubiboot_args_nor=setenv bootargs ${bootargs} ${mtdparts} ubi.mtd=ubipart_nor ubi.fm_autoconvert=1 root=ubi0:rootfs rw rootfstype=ubifs \0"

#define NAND_ENV	\
	"update-fman-ucode-nand-usb=" \
	    "usb start;" \
	    "fatload usb 0 ${load_addr} fsl_fman_ucode_ls1046_r1.0_106_4_18.bin;" \
	    "nand erase.part fman_ucode;" \
	    "nand write ${load_addr} fman_ucode ${filesize};" \
	    "\0" \
	\
	"update-cortina-ucode-nand-usb=" \
        "usb start;" \
        "fatload usb 0 ${loadaddr} cs4315-cs4340-PHY-ucode.txt;" \
        "nand erase.part cortina_ucode;" \
        "nand write ${loadaddr} cortina_ucode ${filesize};" \
        "\0" \
    	\
	"update-ubi-rootfs-nand="\
	    "dhcp;" \
	    "ubi part ubipart_nand;" \
	    "if test $? = \"0\"; then " \
		"tftp ${load_addr} ${TFTP_PATH}/ubi_rootfs_image.nand.ubifs;" \
		"if test $? = \"0\"; then " \
		    "ubi write ${load_addr} rootfs ${filesize};" \
		"fi;" \
	    "fi;" \
	"\0" \
	\
	"ubiboot-nand=" \
	    "ubi part ubipart_nand;" \
	    "ubifsmount ubi0:rootfs;" \
	    "ubifsload ${load_addr} /boot/fitImage;" \
	    "run set_ubiboot_args_nand;" \
	    "bootm ${load_addr}#conf@freescale_qls1046a-4gb-sdk.dtb" \
	    "\0" \
	\
	"set_ubiboot_args_nand=setenv bootargs ${bootargs} ${mtdparts} ubi.mtd=ubipart_nand ubi.fm_autoconvert=1 root=ubi0:rootfs rw rootfstype=ubifs \0"

#ifdef CONFIG_EXTRA_ENV_SETTINGS
#undef CONFIG_EXTRA_ENV_SETTINGS
#endif

#define CONFIG_EXTRA_ENV_SETTINGS \
	"hwconfig=fsl_ddr:bank_intlv=auto\0"	\
	"ramdisk_addr=0x800000\0"		\
	"ramdisk_size=0x2000000\0"		\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"fdt_addr=0x64f00000\0"                 \
	"kernel_addr=0x65000000\0"              \
	"scriptaddr=0x80000000\0"               \
	"scripthdraddr=0x80080000\0"		\
	"fdtheader_addr_r=0x80100000\0"         \
	"kernelheader_addr_r=0x80200000\0"      \
	"load_addr=0xa0000000\0"		\
	"kernel_addr_r=0x81000000\0"            \
	"fdt_addr_r=0x90000000\0"               \
	"ramdisk_addr_r=0xa0000000\0"           \
	"kernel_start=0x1000000\0"		\
	"kernelheader_start=0x800000\0"		\
	"kernel_load=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"kernelheader_size=0x40000\0"		\
	"kernel_addr_sd=0x8000\0"		\
	"kernel_size_sd=0x14000\0"		\
	"kernelhdr_addr_sd=0x4000\0"		\
	"kernelhdr_size_sd=0x10\0"		\
	"console=ttyS0,115200\0"                \
    \
    "ethaddr=02:00:00:ba:be:00\0" \
    "eth1addr=02:00:00:ba:be:01\0" \
    "eth2addr=02:00:00:ba:be:02\0" \
    "eth3addr=02:00:00:ba:be:03\0" \
    "eth4addr=02:00:00:ba:be:04\0" \
    "eth5addr=02:00:00:ba:be:05\0" \
    "eth6addr=02:00:00:ba:be:06\0" \
    \
    "autoload=no\0" \
    "TFTP_PATH=.\0" \
    \
    "mtdids=" MTDIDS_DEFAULT "\0" \
    "mtdparts=" MTDPARTS_DEFAULT "\0" \
    \
    NAND_ENV \
    NOR_ENV \
    SD_ENV \
    \
    "setfans=i2c dev 0; i2c mw 0x2e 0x40 1;i2c mw 0x2e 0x7d 0x2;" \
        "i2c mw 0x2e 0x5c 0xe0;i2c mw 0x2e 0x5d 0xe0;i2c mw 0x2e 0x5e 0xe0;" \
        "i2c mw 0x2e 0x5f 0xc8;i2c mw 0x2e 0x60 0xc8;i2c mw 0x2e 0x61 0xc8;" \
        "i2c mw 0x2e 0x30 0x80;i2c mw 0x2e 0x31 0x80;i2c mw 0x2e 0x32 0x80;\0" \
    \
    "set_netboot_args=run set_ubiboot_args_nand\0" \
    "netboot=dhcp; tftp ${load_addr} ${TFTP_PATH}/fitImage; run set_netboot_args; bootm ${load_addr}#conf@freescale_qls1046a-4gb-sdk.dtb\0"

#include <asm/fsl_secure_boot.h>

#endif /* __QLS1046A_4GB_H__ */
