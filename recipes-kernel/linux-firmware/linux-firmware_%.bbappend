FILESEXTRAPATHS_prepend := "${THISDIR}/linux-firmware:"
SRC_URI_append = " file://wl1271-nvs.bin"

do_install_append () {
	install -m 0644 ${WORKDIR}/wl1271-nvs.bin ${D}/lib/firmware/ti-connectivity/wl1271-nvs.bin
}