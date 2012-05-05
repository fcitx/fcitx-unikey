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

set(UNIKEY_SRCS
    ../unikey/byteio.cpp
    ../unikey/byteio.h
    ../unikey/charset.cpp
    ../unikey/charset.h
    ../unikey/convert.cpp
    ../unikey/data.cpp
    ../unikey/data.h
    ../unikey/error.cpp
    ../unikey/inputproc.cpp
    ../unikey/inputproc.h
    ../unikey/keycons.h
    ../unikey/mactab.cpp
    ../unikey/mactab.h
    ../unikey/pattern.cpp
    ../unikey/pattern.h
    ../unikey/ukengine.cpp
    ../unikey/ukengine.h
    ../unikey/unikey.cpp
    ../unikey/unikey.h
    ../unikey/usrkeymap.cpp
    ../unikey/usrkeymap.h
    ../unikey/vnconv.h
    ../unikey/vnlexi.h
)

add_definitions( -DLOCALEDIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\" )

fcitx_add_addon(fcitx-unikey ${fcitx_unikey_sources} ${UNIKEY_SRCS} )
