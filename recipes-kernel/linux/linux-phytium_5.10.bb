SUMMARY = "phytium bsp linux 5.1"
DESCRIPTION = "This file is a file used to support the phytium e2000 device 5.1 kernel"
LICENSE = "MIT"

require recipes-kernel/linux/linux-yocto.inc
require linux-phytium.inc

LINUX_VERSION ?= "5.10"

SRC_URI:append = " ${@bb.utils.contains('IMAGE_FSTYPES', 'ext4', \
		   'file://${THISDIR}/files/ext4.cfg', \
		   'file://${THISDIR}/linux-phytium_5.10/', \
		   '', \
		   d)}"


do_patch:append() {
	sed -i 's/-I\($(BCMDHD_ROOT)\)/-I$(srctree)\/\1/g' \
		${S}/drivers/net/wireless/rockchip_wlan/rkwifi/bcmdhd/Makefile
}