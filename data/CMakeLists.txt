if (QT_FOUND)
    set(MACRO_EDIT "program:fcitx-unikey-macro-editor")
else (QT_FOUND)
    set(MACRO_EDIT "native:unikey/macro")
endif (QT_FOUND)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fcitx-unikey.conf.in.in ${CMAKE_CURRENT_BINARY_DIR}/fcitx-unikey.conf.in)
fcitx_add_addon_conf_file(${CMAKE_CURRENT_BINARY_DIR}/fcitx-unikey.conf)
fcitx_add_configdesc_file(fcitx-unikey.desc)
fcitx_add_inputmethod_conf_file(unikey.conf)

install(FILES unikey.png DESTINATION ${FCITX4_PREFIX}/share/fcitx/skin/default/)
install(FILES unikey.png RENAME fcitx-unikey.png DESTINATION share/icons/hicolor/256x256/apps/)