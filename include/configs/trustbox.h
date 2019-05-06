#ifndef __TRUSTBOX_H__
#define __TRUSTBOX_H__

#include "ls1012a_common.h"

/* DDR */
#define CONFIG_DIMM_SLOTS_PER_CTLR	1
#define CONFIG_CHIP_SELECTS_PER_CTRL	1
#define CONFIG_SYS_SDRAM_SIZE		0x40000000
#define CONFIG_CMD_MEMINFO
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

/* SPI */
#define CONFIG_FSL_DSPI1
#define CONFIG_DEFAULT_SPI_BUS				0
#define MMAP_DSPI					DSPI1_BASE_ADDR
#define CONFIG_SYS_DSPI_CTAR0   			1
#define CONFIG_SYS_DSPI_CTAR1				1
#define CONFIG_SF_DEFAULT_SPEED				10000000
#define CONFIG_SF_DEFAULT_MODE				SPI_MODE_0
#define CONFIG_SF_DEFAULT_BUS				0
#define CONFIG_SF_DEFAULT_CS				0

/* Size of malloc() pool */
#undef CONFIG_SYS_MALLOC_LEN
#define CONFIG_SYS_MALLOC_LEN		(0x60000 + 128 * 1024 + 0x100000)

#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_BAR

/*
 * I2C IO expander
 */
#define I2C_MUX_IO_ADDR		0x24
#define I2C_MUX_IO2_ADDR	0x25
#define I2C_MUX_IO_0		0
#define I2C_MUX_IO_1		1
#define SW_BOOT_MASK		0x03
#define SW_BOOT_EMU		0x02
#define SW_BOOT_BANK1		0x00
#define SW_BOOT_BANK2		0x01
#define SW_REV_MASK		0xF8
#define SW_REV_A		0xF8
#define SW_REV_B		0xF0
#define SW_REV_C		0xE8
#define SW_REV_C1		0xE0
#define SW_REV_C2		0xD8
#define SW_REV_D		0xD0
#define SW_REV_E		0xC8
#define __PHY_MASK		0xF9
#define __PHY_ETH2_MASK		0xFB
#define __PHY_ETH1_MASK		0xFD

/*  MMC  */
#ifdef CONFIG_MMC
#define CONFIG_SYS_FSL_MMC_HAS_CAPBLT_VS33
#endif

/* Mtdparts configuration */
#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITIONS
#define CONFIG_CMD_MTDPARTS
#define CONFIG_USE_SPIFLASH
#define CONFIG_SPI_FLASH_MTD

/* QSPI */
#ifdef CONFIG_QSPI_BOOT
#define CONFIG_SYS_QE_FW_IN_SPIFLASH
#define CONFIG_SYS_FMAN_FW_ADDR				0x400d0000
#define CONFIG_ENV_SPI_BUS					0
#define CONFIG_ENV_SPI_CS					0
#define CONFIG_ENV_SPI_MAX_HZ				1000000
#define CONFIG_ENV_SPI_MODE					0x03
#define CONFIG_FSL_SPI_INTERFACE
#define CONFIG_SF_DATAFLASH
#define CONFIG_FSL_QSPI
#define QSPI0_AMBA_BASE						0x40000000
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_BAR
#define CONFIG_SPI_FLASH_SST
#endif /* CONFIG_QSPI_BOOT */

/* UBI filesystem support */
#define CONFIG_CMD_UBI
#define CONFIG_CMD_UBIFS
#define CONFIG_RBTREE
#define CONFIG_LZO
#define CONFIG_MTD_UBI_WL_THRESHOLD	4096
#define CONFIG_MTD_UBI_BEB_LIMIT	20


#define CONFIG_PCIE1		/* PCIE controller 1 */

#define CONFIG_PCI_SCAN_SHOW

#define CONFIG_CMD_MEMINFO
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		0x9fffffff

#define MTDIDS_DEFAULT \
	"nor0=1550000.quadspi"

#define MTDPARTS_DEFAULT    \
	"mtdparts=1550000.quadspi:" \
		"2M@0x0(u-boot)," \
		"256k(env)," \
		"256k(pfe)," \
		"1M(ppa)," \
		"256k(u-boot_hdr)," \
		"256k(ppa_hdr)," \
		"-(UBI)"

#ifndef CONFIG_SPL_BUILD
#undef BOOT_TARGET_DEVICES
#define BOOT_TARGET_DEVICES(func) \
	func(MMC, mmc, 0) \
	func(USB, usb, 0) \
/*	func(SCSI, scsi, 0) \	*/
/*	func(DHCP, dhcp, na)	*/
#endif

/* Default environment variables */
#define COMMON_UBOOT_CONFIG \
	"update_tftp_rcw_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/rcw.bin;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 0 10000;" \
		"sf write $load_addr 0 $filesize;" \
        "fi\0" \
	"update_tftp_uboot_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/u-boot.bin;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 10000 1F0000;" \
		"sf write $load_addr 10000 $filesize;" \
        "fi\0" \
	"update_tftp_uboot_hdr_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/hdr_uboot.out;" \
        "if test $? = \"0\"; then " \
               "sf probe 0:0;" \
               "sf erase u-boot_hdr 40000;" \
               "sf write $load_addr u-boot_hdr $filesize;" \
        "fi\0" \
	"update_tftp_ppa_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/ppa.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase ppa 100000;" \
		"sf write $load_addr ppa $filesize;" \
        "fi\0" \
	"update_tftp_ppa_hdr_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/hdr_ppa.out;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase ppa_hdr 40000;" \
		"sf write $load_addr ppa_hdr $filesize;" \
        "fi\0" \
	"update_tftp_pfe_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/pfe_fw_sbl.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase pfe 40000;" \
		"sf write $load_addr pfe $filesize;" \
        "fi\0" \
	"update_tftp_image_qspi_nor=" \
        "dhcp;" \
        "tftp $load_addr $update_files_path/trustbox_qspi_fw.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 0 0x04000000;" \
		"sf write $load_addr 0 $filesize;" \
        "fi\0" \
	"update_usb_rcw_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/rcw.bin;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 0 10000;" \
		"sf write $load_addr 0 $filesize;" \
        "fi\0" \
	"update_usb_uboot_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/u-boot.bin;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 10000 1F0000;" \
		"sf write $load_addr 10000 $filesize;" \
        "fi\0" \
	"update_usb_uboot_hdr_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/hdr_uboot.out;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase u-boot_hdr 40000;" \
		"sf write $load_addr u-boot_hdr $filesize;" \
        "fi\0" \
	"update_usb_ppa_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/ppa.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase ppa 100000;" \
		"sf write $load_addr ppa $filesize;" \
        "fi\0" \
	"update_usb_ppa_hdr_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/hdr_ppa.out;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase ppa_hdr 40000;" \
		"sf write $load_addr ppa_hdr $filesize;" \
        "fi\0" \
	"update_usb_pfe_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/pfe_fw_sbl.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase pfe 40000;" \
		"sf write $load_addr pfe $filesize;" \
        "fi\0" \
	"update_usb_image_qspi_nor=" \
        "usb start;" \
        "fatload usb 0:1 $load_addr $update_files_path/trustbox_qspi_fw.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 0 0x04000000;" \
		"sf write $load_addr 0 $filesize;" \
        "fi\0" \
	"update_mmc_rcw_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/rcw.bin;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 0 10000;" \
		"sf write $load_addr 0 $filesize;" \
        "fi\0" \
	"update_mmc_uboot_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/u-boot.bin;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 10000 1F0000;" \
		"sf write $load_addr 10000 $filesize;" \
        "fi\0" \
	"update_mmc_uboot_hdr_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/hdr_uboot.out;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase u-boot_hdr 40000;" \
		"sf write $load_addr u-boot_hdr $filesize;" \
        "fi\0" \
	"update_mmc_ppa_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/ppa.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase ppa 100000;" \
		"sf write $load_addr ppa $filesize;" \
        "fi\0" \
	"update_mmc_ppa_hdr_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/hdr_ppa.out;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase ppa_hdr 40000;" \
		"sf write $load_addr ppa_hdr $filesize;" \
        "fi\0" \
	"update_mmc_pfe_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/pfe_fw_sbl.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase pfe 40000;" \
		"sf write $load_addr pfe $filesize;" \
        "fi\0" \
	"update_mmc_image_qspi_nor=" \
        "mmc rescan;" \
        "ext4load mmc 0:1 $load_addr $update_files_path/trustbox_qspi_fw.itb;" \
        "if test $? = \"0\"; then " \
		"sf probe 0:0;" \
		"sf erase 0 0x04000000;" \
		"sf write $load_addr 0 $filesize;" \
        "fi\0" \
	"usbboot=" \
	"usb start;" \
	"ext4load usb 0:1 $fdt_addr_r /boot/trustbox.dtb;" \
	"ext4load usb 0:1 $kernel_addr_r /boot/uImage;" \
	"setenv bootargs \"console=ttyS0,115200 earlycon=uart8250,mmio,0x21c0500 lpj=250000 loglevel=2 noinitrd  root=/dev/sda1 rootfstype=ext4 rw rootwait $mtdparts\";" \
	"if test $? = \"0\"; then " \
		"pfe stop;" \
		"bootm $kernel_addr_r - $fdt_addr_r;" \
	"fi\0" \

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS		\
	"verify=no\0"				\
	"fdt_high=0xffffffffffffffff\0"		\
	"initrd_high=0xffffffffffffffff\0"	\
	"fdt_addr=0x00f00000\0"			\
	"kernel_addr=0x01000000\0"		\
	"kernelheader_addr=0x800000\0"		\
	"scriptaddr=0x80000000\0"		\
	"scripthdraddr=0x80080000\0"		\
	"fdtheader_addr_r=0x80100000\0"		\
	"kernelheader_addr_r=0x80200000\0"	\
	"kernel_addr_r=0x81000000\0"		\
	"fdt_addr_r=0x90000000\0"		\
	"load_addr=0xa0000000\0"		\
	"kernel_size=0x2800000\0"		\
	"kernelheader_size=0x40000\0"		\
	"console=ttyS0,115200\0"		\
	"ethprime=pfe_eth0\0" \
	"ethaddr=02:00:00:ba:be:01\0" \
	"eth1addr=02:00:00:ba:be:02\0" \
	"update_files_path=.\0" \
	"autoload=no\0" \
	COMMON_UBOOT_CONFIG \
	BOOTENV					\
	"boot_scripts=trustbox_boot.scr trustbox_recovery.scr\0"	\
	"default_bootargs=root=/dev/mmcblk0p1 rootfstype=ext4 rw rootwait $mtdparts\0" \
	"default_boot=" \
			  "setenv load_succes 1;"\
			  "ext4load mmc 0:1 $fdt_addr_r /boot/trustbox.dtb;" \
			  "if test $? = \"0\"; then " \
							 "setenv load_succes 0;"\
			  "fi;"\
			  "env exists secureboot && ext4load mmc 0:1 $fdtheader_addr_r /boot/hdr_dtb.out;"\
			  "env exists secureboot && esbc_validate $fdtheader_addr_r || esbc_halt;"\
			  "ext4load mmc 0:1 $kernel_addr_r /boot/uImage;" \
			  "if test $? = \"0\"; then " \
							 "setenv load_succes 0;"\
			  "fi;"\
			  "env exists secureboot && ext4load mmc 0:1 $kernelheader_addr_r /boot/hdr_kernel.out; " \
			  "env exists secureboot && esbc_validate $kernelheader_addr_r || esbc_halt;" \
			  "if test $load_succes = \"0\"; then " \
							 "pfe stop;" \
							 "setenv bootargs $bootargs $default_bootargs;" \
							 "bootm $kernel_addr_r - $fdt_addr_r;" \
			  "fi\0" \

#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND	"run distro_bootcmd; run default_boot"

#include <asm/fsl_secure_boot.h>

#endif /* __TRUSTBOX_H__ */
