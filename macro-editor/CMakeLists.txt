

set(MACRO_EDITOR_SRCS
    model.cpp
    main.cpp
    editor.cpp
    dialog.cpp
    )
set(MACRO_EDITOR_HDRS
    model.h
    editor.h
    dialog.h
    main.h
    )
fcitx_translate_add_sources(
    ${MACRO_EDITOR_SRCS}
    ${MACRO_EDITOR_HDRS})

if (NOT QT_FOUND OR NOT FCITX_QT_FOUND OR NOT ENABLE_QT)
    return()
endif (NOT QT_FOUND OR NOT FCITX_QT_FOUND OR NOT ENABLE_QT)

include_directories(
    ${FCITX_QT_INCLUDE_DIRS}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/unikey
    )

link_directories(
${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
${FCITX_QT_LIBRARY_DIRS}
)
qt4_wrap_cpp(MACRO_EDITOR_MOCS ${MACRO_EDITOR_HDRS})
qt4_wrap_ui(MACRO_EDITOR_SRCS editor.ui dialog.ui)
add_library(fcitx-unikey-macro-editor
  MODULE ${MACRO_EDITOR_SRCS} ${MACRO_EDITOR_MOCS})
set_target_properties(fcitx-unikey-macro-editor PROPERTIES
                      LINK_FLAGS "-Wl,--no-undefined"
)
target_link_libraries(fcitx-unikey-macro-editor
    debug ${QT_QTCORE_LIBRARY_DEBUG}
    debug ${QT_QTDBUS_LIBRARY_DEBUG}
    debug ${QT_QTGUI_LIBRARY_DEBUG}
    optimized ${QT_QTCORE_LIBRARY_RELEASE}
    optimized ${QT_QTDBUS_LIBRARY_RELEASE}
    optimized ${QT_QTGUI_LIBRARY_RELEASE}
    unikey
    ${FCITX4_FCITX_UTILS_LIBRARIES}
    ${FCITX4_FCITX_CONFIG_LIBRARIES}
    ${FCITX_QT_LIBRARIES}
    LibIntl::LibIntl
    )

install(TARGETS fcitx-unikey-macro-editor DESTINATION ${FCITX4_ADDON_INSTALL_DIR}/qt)
