
if (QT_FOUND)
    set(MACRO_EDIT "program:fcitx-unikey-macro-editor")
else (QT_FOUND)
    set(MACRO_EDIT "native:unikey/macro")
endif (QT_FOUND)


configure_file(${CMAKE_CURRENT_SOURCE_DIR}/fcitx-unikey.conf.in.in ${CMAKE_CURRENT_BINARY_DIR}/fcitx-unikey.conf.in)

include_directories (
${PROJECT_SOURCE_DIR}/unikey
${FCITX4_FCITX_INCLUDE_DIRS}
${FCITX4_FCITX_CONFIG_INCLUDE_DIRS}
${FCITX4_FCITX_UTILS_INCLUDE_DIRS}
${PROJECT_BINARY_DIR}
${LIBICONV_INCLUDE_DIR}
)

set( fcitx_unikey_sources
    unikey-im.cpp
    unikey-config.cpp
    unikey-ui.cpp
    )

add_definitions( -DLOCALEDIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\" )

fcitx_add_addon_full(unikey
   SOURCES ${fcitx_unikey_sources}
   LINK_LIBS unikey
   CONF_SRC ${CMAKE_CURRENT_BINARY_DIR}/fcitx-unikey.conf
   )
