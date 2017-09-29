

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

if (NOT ENABLE_QT)
    return()
endif (NOT ENABLE_QT)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/unikey
    )

link_directories(
${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
)
qt5_wrap_ui(MACRO_EDITOR_SRCS editor.ui dialog.ui)
add_library(fcitx-unikey-macro-editor
  MODULE ${MACRO_EDITOR_SRCS} ${MACRO_EDITOR_MOCS})
set_target_properties(fcitx-unikey-macro-editor PROPERTIES
                      LINK_FLAGS "-Wl,--no-undefined"
                      AUTOMOC TRUE
)
target_link_libraries(fcitx-unikey-macro-editor
    Qt5::Core
    Qt5::Widgets
    FcitxQt5::WidgetsAddons
    LibIntl::LibIntl
    unikey
    ${FCITX4_FCITX_UTILS_LIBRARIES}
    ${FCITX4_FCITX_CONFIG_LIBRARIES}
    )

install(TARGETS fcitx-unikey-macro-editor DESTINATION ${FCITX4_ADDON_INSTALL_DIR}/qt)
