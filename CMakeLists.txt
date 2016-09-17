cmake_minimum_required(VERSION 2.6)

project(fcitx-unikey)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

option(ENABLE_QT "Enable Qt based macro editor" On)

# uninstall target
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")

find_package(Fcitx 4.2.8 REQUIRED)
find_package(Gettext REQUIRED)
find_package(LibIntl REQUIRED)

if (ENABLE_QT)
find_package(Qt4 4.8)

pkg_check_modules(FCITX_QT "fcitx-qt>=4.2.8")
endif (ENABLE_QT)

configure_file(config.h.in config.h)

add_subdirectory(po)
add_subdirectory(unikey)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(macro-editor)
