# Copyright (C) 2015 PHYTEC Messtechnik GmbH,
# Author: Stefan Christ <s.christ@phytec.de>

inherit phygittag
inherit buildinfo
require barebox.inc

FILESEXTRAPATHS_prepend := "${THISDIR}/defconfigs:${THISDIR}/defconfigs/features:"
FILESEXTRAPATHS_prepend := "${THISDIR}/env-2015.07.0-phy2:"

GIT_URL = "git://git.phytec.de/${PN}"
SRC_URI = "${GIT_URL};branch=${BRANCH}"
SRC_URI_append = " \
    file://no-blspec.cfg \
    file://md5sum.cfg \
    file://commonenv \
    file://boardenv \
    file://machineenv \
"

S = "${WORKDIR}/git"

PR = "${INC_PR}.0"

# NOTE: Keep version in filename in sync with commit id!
SRCREV = "b56b7df97c6095d1317aed49b482b5457951f5e3"

COMPATIBLE_MACHINE  =  "phyflex-imx6-1"
COMPATIBLE_MACHINE .= "|phyflex-imx6-2"
COMPATIBLE_MACHINE .= "|phyflex-imx6-3"
COMPATIBLE_MACHINE .= "|phyflex-imx6-4"
COMPATIBLE_MACHINE .= "|phyflex-imx6-5"
COMPATIBLE_MACHINE .= "|phyflex-imx6-6"
COMPATIBLE_MACHINE .= "|phyflex-imx6-7"
COMPATIBLE_MACHINE .= "|phyflex-imx6-8"
COMPATIBLE_MACHINE .= "|phyflex-imx6-9"
COMPATIBLE_MACHINE .= "|phyflex-imx6-10"

COMPATIBLE_MACHINE .= "|phycard-imx6-1"
COMPATIBLE_MACHINE .= "|phycard-imx6-2"

COMPATIBLE_MACHINE .= "|phyboard-alcor-imx6-1"
COMPATIBLE_MACHINE .= "|phyboard-subra-imx6-1"

COMPATIBLE_MACHINE .= "|phyboard-mira-imx6-3"
COMPATIBLE_MACHINE .= "|phyboard-mira-imx6-4"