require libgles-omap3-no-x.inc

LICENSE = "TI-TSPA"

PR = "${INC_PR}.1"

BINLOCATION_omap3 = "${S}/gfx_rel_es3.x"
BINLOCATION_ti816x = "${S}/gfx_rel_es6.x"
BINLOCATION_ti814x = "${S}/gfx_rel_es6.x"
BINLOCATION_ti33x = "${S}/gfx_rel_es8.x"
BINLOCATION_ti43x = "${S}/gfx_rel_es9.x"

PLATFORM = "LinuxARMV7"
PVR_INIT = "pvrsrvctl"

SGXPV = "5_01_01_02"
IMGPV = "1.10.2359475"

TI_BIN_UNPK_WDEXT := "/Graphics_SDK_${SGXPV}"

BINFILE_HARDFP = "Graphics_SDK_setuplinux_hardfp_${SGXPV}.bin"
MD5SUM_HARDFP = "94bcb31ea7eb50df1dfa4037055b638e"
SHA256SUM_HARDFP = "54641222cdb49b03f996cbd6412de227198d9e084f5647d706bbf4217e8cdb07"

BINFILE := "${BINFILE_HARDFP}"

SRC_URI = "http://software-dl.ti.com/dsps/dsps_public_sw/gfxsdk/${SGXPV}/exports/${BINFILE} \
           file://99-bufferclass.rules  \
"

SRC_URI[md5sum] := "${MD5SUM_HARDFP}"
SRC_URI[sha256sum] := "${SHA256SUM_HARDFP}"

S = "${WORKDIR}/Graphics_SDK_${SGXPV}"

LIBGLESWINDOWSYSTEM ?= "libpvrPVR2D_FRONTWSEGL.so.1"