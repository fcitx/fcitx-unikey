include_directories(
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTGUI_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/unikey
    )

link_directories(
${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
)

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
    )
qt4_wrap_cpp(MACRO_EDITOR_MOCS ${MACRO_EDITOR_HDRS})
qt4_wrap_ui(MACRO_EDITOR_SRCS editor.ui dialog.ui)
add_executable(fcitx-unikey-macro-editor ${MACRO_EDITOR_SRCS} ${MACRO_EDITOR_MOCS})
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
    )

install(TARGETS fcitx-unikey-macro-editor DESTINATION ${FCITX4_PREFIX}/bin)