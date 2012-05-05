fcitx_add_addon_conf_file(fcitx-unikey.conf)
fcitx_add_configdesc_file(fcitx-unikey.desc)
fcitx_add_inputmethod_conf_file(unikey.conf)

install(FILES unikey.png DESTINATION ${FCITX4_PREFIX}/share/fcitx/skin/default/)
install(FILES unikey.png RENAME fcitx-unikey.png DESTINATION share/icons/hicolor/256x256/apps/)