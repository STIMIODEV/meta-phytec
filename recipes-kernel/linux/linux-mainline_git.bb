# Copyright (C) 2014 Stefan Mueller-Klieser <s.mueller-klieser@phytec.de>
# PHYTEC Messtechnik GmbH
require common/recipes-kernel/linux/linux.inc

FILESEXTRAPATHS_prepend := "${THISDIR}/linux-mainline:${THISDIR}/linux-mainline/features:"

SRC_URI = "git://git.phytec.de/linux-mainline;branch=${BRANCH}"
SRC_URI_append = " \
    file://defconfig \
"

#SRCREV = "${AUTOREV}"
#PV = "${LINUX_VERSION}-git${SRCPV}"

PV = "v3.17.6-phy1"
# TAG = ${PV}
SRCREV = "b866874890884ddf526b8225e706f46c5856c698"

LINUX_VERSION ?= "3.17.6-phy"
BRANCH ?= "v3.17.6-phy"
KERNEL_LOCALVERSION = "-${BSP_VERSION}"

COMPATIBLE_MACHINE_mx6 = "(mx6)"
