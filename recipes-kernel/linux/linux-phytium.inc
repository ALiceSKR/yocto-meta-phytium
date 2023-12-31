# Copyright (C) 2019, Fuzhou Phytium Electronics Co., Ltd
# Released under the MIT license (see COPYING.MIT for the terms)

inherit python3-dir

DEPENDS:append = " openssl-native lz4-native ${PYTHON_PN}-native"

LINUX_VERSION_EXTENSION ?= "-phytium-${LINUX_KERNEL_TYPE}"

PATCHPATH = "${THISDIR}/${BPN}_${LINUX_VERSION}"
inherit auto-patch

KCONFIG_MODE ?= "--alldefconfig"

# Make sure we use /usr/bin/env ${PYTHON_PN} for scripts
do_patch:append() {
	for s in `grep -rIl python ${S}/scripts`; do
		sed -i -e '1s|^#!.*python[23]*|#!/usr/bin/env ${PYTHON_PN}|' $s
	done
}

do_compile:prepend() {
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${STAGING_LIBDIR_NATIVE}
}

do_compile_kernelmodules:prepend() {
	export PKG_CONFIG_DIR="${STAGING_DIR_NATIVE}${libdir_native}/pkgconfig"
	export PKG_CONFIG_PATH="$PKG_CONFIG_DIR:${STAGING_DATADIR_NATIVE}/pkgconfig"
	export PKG_CONFIG_LIBDIR="$PKG_CONFIG_DIR"
	export PKG_CONFIG_SYSROOT_DIR=""
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${STAGING_LIBDIR_NATIVE}
}

# Hack for phytium style images
KERNEL_IMAGETYPES:append = \
	"${@' boot.img zboot.img' if d.getVar('PHYTIUM_KERNEL_IMAGES') == '1' else ''}"
python () {
    if not d.getVar('KERNEL_DEVICETREE'):
        raise bb.parse.SkipPackage('KERNEL_DEVICETREE is not specified!')

    if d.getVar('PHYTIUM_KERNEL_IMAGES'):
        # Use phytium stype target, which is '<dts(w/o suffix)>.img'
        d.setVar('KERNEL_IMAGETYPE_FOR_MAKE', ' ' + d.getVar('KERNEL_DEVICETREE').replace('phytium/', '').replace('.dtb', '.img'));
}

do_kernel_metadata:prepend() {
	# Force regenerating defconfig
	rm -f ${WORKDIR}/defconfig

	# Support defconfig fragments
	cd "${S}/arch/${ARCH}/configs"
	if [ ! -f "${KBUILD_DEFCONFIG}" ]; then
		DEFCONFIGS="phytium_linux_defconfig ${SOC_FAMILY}_linux.config"
		[ "$KBUILD_DEFCONFIGS" ] || KBUILD_DEFCONFIGS="$DEFCONFIGS"

		echo "Merging $KBUILD_DEFCONFIGS into $KBUILD_DEFCONFIG..."
		cat ${KBUILD_DEFCONFIGS:-$CONFIGS} > ${KBUILD_DEFCONFIG} || true
	fi
	cd -
}

# Link phytium style images
do_install:prepend() {
	for image in $(ls "${B}/" | grep ".img$"); do
		ln -rsf ${B}/${image} ${B}/arch/${ARCH}/boot/
	done

	if [ "${PHYTIUM_KERNEL_COMPRESSED}" = "1" ]; then
		if [ -r "${B}/zboot.img" ]; then
			ln -rsf ${B}/zboot.img ${B}/arch/${ARCH}/boot/boot.img
		fi
	fi
}
