
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
   IM_CONFIG unikey.conf
   )
