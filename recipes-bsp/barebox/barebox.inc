# Copyright (C) 2014 Stefan Mueller-Klieser <s.mueller-klieser@phytec.de>
# PHYTEC Messtechnik GmbH
#
# TODO: see if there is some development towards moving all kconfig
#       funcionality into the kconfig-frontends package and adjust
#       the recipe accordingly.
#
SUMMARY = "The Barebox Bootloader"
DESCRIPTION = "Barebox (formerly known as u-boot-v2) is a bootloader that \
               inherits the best of U-Boot and the Linux kernel: The size and \
	       look-and-feel of u-boot, with driver model and lots of design \
	       concepts from the kernel. \
	       The bb recipe adds config fragment support and kconfig \
	       command handling using the yocto kern-tools-native and \
	       kconfig-frontends-native."
HOMEPAGE = "http://www.barebox.org"
SECTION = "bootloaders"
PROVIDES = "virtual/bootloader"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING;md5=057bf9e50e1ca857d0eb97bfe4ba8e5d"
SRC_URI = "http://barebox.org/download/barebox-${PV}.tar.bz2"
SRC_URI_append = " \
    file://0001-Makefile-add-TARGETCC.patch \
    ${@bb.utils.contains("EXTRA_IMAGE_FEATURES", "debug-tweaks", "file://far.env.bin", "", d)} \
"
FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

inherit kernel-arch cml1 deploy
inherit kconfig-prepare-configure

# Increase INC_PR everytime the include file is changed.
INC_PR = "r2"

PACKAGE_ARCH = "${MACHINE_ARCH}"
# verbose build 0/1
VERBOSE_BUILD ?= "0"
EXTRA_OEMAKE = 'TARGETCC="${CC}" V=${VERBOSE_BUILD} CROSS_COMPILE=${TARGET_PREFIX} CC="${TARGET_PREFIX}gcc ${TOOLCHAIN_OPTIONS}"'
# target rootfs boot directory
base_bootdir = "/boot"
PATCHTOOL = "git"

DEPENDS += "lzop-native"

BAREBOX_LOCALVERSION ??= ""
# name of the image created by barebox Makefile
BAREBOX_BIN ?= "barebox.bin"
# filename of file to be deployed by this recipe
BAREBOX_IMAGE_BASE_NAME ?= "barebox-${MACHINE}-${PKGV}-${PKGR}-${DATETIME}"
BAREBOX_IMAGE_BASE_NAME[vardepsexclude] += "DATETIME"
BAREBOX_BIN_SYMLINK ?= "barebox.bin"
BAREBOX_DEFAULT_DEFCONFIG ?= ""
# barebox environment
BAREBOX_DEFAULT_ENV ?= ""

# recipe private variables
BAREBOX_COMMON_ENVDIR = "${WORKDIR}/commonenv"
BAREBOX_ENVDIR = "${WORKDIR}/environment"


python do_prepare_configure_append() {
    localversion = d.getVar('BAREBOX_LOCALVERSION', True)
    bb.note("Setting LOCALVERSION in .config to %s" % localversion)
    cmd = 'kconfig-tweak --file %s --set-str LOCALVERSION "%s"' % (config, localversion)
    subprocess.check_output(cmd, stderr=subprocess.STDOUT, shell=True)

    # Add environment directories to .config if they exist
    env_dirs = ""
    for env_dir in [".commonenv", ".environment"]:   # order is important!
       if os.path.isdir(os.path.join(B, env_dir)):
           env_dirs += " " + env_dir

    if env_dirs.strip() != "":
        ret = subprocess.call(["kconfig-tweak", "--file", config,
                               "--set-str", "DEFAULT_ENVIRONMENT_PATH", env_dirs])
        if ret != 0:
            bb.error("Cannot set DEFAULT_ENVIRONMENT_PATH. kconfig-tweak exit code is %d." % (ret))

}

# its possible to give two environment folders to barebox: commonenv and
# environment, if an environment directory is provided, use it
python do_prepare_env() {
    import shutil, subprocess
    S = d.getVar('S', True)
    env_dir = d.getVar('BAREBOX_ENVDIR', True)
    common_env_dir = d.getVar('BAREBOX_COMMON_ENVDIR', True)
    if not (os.path.isdir(common_env_dir) or os.path.isdir(env_dir)):
        bb.note("No environment directory provided, using BAREBOX_DEFAULT_ENV")
        common_env_dir = d.getVar('BAREBOX_DEFAULT_ENV', True)
        if not common_env_dir:
            bb.note("no BAREBOX_DEFAULT_ENV provided, creating empty default")
            bb.utils.mkdirhier(os.path.join(S, '.commonenv'))
    if os.path.isdir(common_env_dir):
        bb.note("copying common environment to source directory")
        oe.path.copytree(common_env_dir, oe.path.join(S, '.commonenv'))
    if os.path.isdir(env_dir):
        bb.note("copying environment to source directory")
        oe.path.copytree(env_dir, oe.path.join(S, '.environment'))

    # Install the script far into the barebox environment. It will only be
    # added, if you enable "debug-tweaks" in EXTRA_IMAGE_FEATURES in your
    # local.conf.
    workdir_far_env_bin = os.path.join("${WORKDIR}", "far.env.bin")
    if os.path.isfile(workdir_far_env_bin):
        bb.utils.mkdirhier(os.path.join(S, ".environment/bin"))
        shutil.copyfile(workdir_far_env_bin,
                        os.path.join(S, ".environment/bin", "far"))
}
addtask prepare_env after do_patch before do_prepare_configure

do_default_defconfig () {
	bbnote "generating .config for target ${BAREBOX_DEFAULT_DEFCONFIG}"
	unset CFLAGS LDFLAGS
	oe_runmake ${BAREBOX_DEFAULT_DEFCONFIG}
}

do_savedefconfig() {
	bbplain "saving defconfig to ${WORKDIR}/defconfig.temp"
	oe_runmake savedefconfig
	install -m 644 ${B}/defconfig ${WORKDIR}/defconfig.temp
}
addtask savedefconfig after do_configure

do_compile () {
	unset CFLAGS LDFLAGS
	oe_runmake all
}

do_install () {
	bbnote "Installing barebox image on target rootfs"
	install -d ${D}${base_bootdir}
	install ${B}/${BAREBOX_BIN} ${D}${base_bootdir}/${BAREBOX_IMAGE_BASE_NAME}.bin
	ln -sf ${BAREBOX_IMAGE_BASE_NAME}.bin ${D}${base_bootdir}/${BAREBOX_BIN_SYMLINK}

	if [ -e ${B}/scripts/bareboxenv-target ] ; then
		bbnote "Installing target env tool"
		install -d ${D}${base_sbindir}
		install -m 744 ${B}/scripts/bareboxenv-target ${D}${base_sbindir}/bareboxenv
	fi

	if [ -e ${B}/scripts/bareboxcrc32-target ] ; then
		bbnote "Installing target crc32 tool"
		install -d ${D}${base_sbindir}
		install -m 744 ${B}/scripts/bareboxcrc32-target ${D}${base_sbindir}/bareboxcrc32
	fi

	if [ -e ${B}/scripts/kernel-install-target ] ; then
		bbnote "Installing target kernel-install tool"
		install -d ${D}${base_sbindir}
		install -m 744 ${B}/scripts/kernel-install-target ${D}${base_sbindir}/barebox-kernel-install
	fi
}
do_install[prefuncs] += "package_get_auto_pr"
FILES_${PN} = ""
FILES_${PN} += "${base_bootdir}"
PACKAGES =+ "${PN}-targettools"
FILES_${PN}-targettools += "${base_sbindir}/barebox*"

do_deploy () {
	bbnote "Deploying barebox"
	install -m 644 ${B}/${BAREBOX_BIN} ${DEPLOYDIR}/${BAREBOX_IMAGE_BASE_NAME}.bin
	ln -sf ${BAREBOX_IMAGE_BASE_NAME}.bin ${DEPLOYDIR}/${BAREBOX_BIN_SYMLINK}

	bbnote "Installing host barebox toolset"
	install -d ${STAGING_BINDIR_NATIVE}
	install -m 755 ${B}/scripts/bareboxenv ${STAGING_BINDIR_NATIVE}/

	install -m 644 ${B}/.config ${DEPLOYDIR}/${BAREBOX_IMAGE_BASE_NAME}.config
	ln -sf ${BAREBOX_IMAGE_BASE_NAME}.config ${DEPLOYDIR}/${PN}.config
}
do_deploy[prefuncs] += "package_get_auto_pr"
addtask deploy before do_build after do_compile