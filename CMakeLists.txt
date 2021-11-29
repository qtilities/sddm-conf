cmake_minimum_required(VERSION 3.5)
project(sddm-conf VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(UPDATE_TRANSLATIONS "Update source translations" OFF)

# TODO: It seems that Qt6 is not (yet) supported in LXQt translations

#find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
include(GNUInstallDirs REQUIRED)

set(APPSTREAM_ID "sddm_conf")
set(DESKTOP_FILE_IN
    resources/${APPSTREAM_ID}.desktop.in
)
set(UI_FILES
    src/maindialog.ui
)
set(SOURCE_FILES
    src/application.hpp
    src/application.cpp
    src/maindialog.hpp
    src/maindialog.cpp
    src/settings.hpp
    src/settings.cpp
    ${UI_FILES}
)
#===============================================================================
# Translations
#===============================================================================
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(LXQT_CMAKE_MODULES_DIR "${CMAKE_MODULE_PATH}/LXQTBuildTools")

include(LXQTBuildTools/LXQtTranslateDesktop)
include(LXQTBuildTools/LXQtTranslateTs)

lxqt_translate_ts(QM_FILES
    UPDATE_TRANSLATIONS ${UPDATE_TRANSLATIONS}
    SOURCES ${SOURCE_FILES}
    TRANSLATION_DIR "resources/translations"
    INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/${CMAKE_PROJECT_NAME}/translations
)
lxqt_translate_desktop(DESKTOP_FILE
    SOURCES ${DESKTOP_FILE_IN}
    TRANSLATION_DIR "resources/translations"
    USE_YAML
)
qt5_wrap_ui(UI_SOURCE ${UI_FILES})
set_property(SOURCE ${UI_SOURCE} PROPERTY SKIP_AUTOMOC ON)
#===============================================================================
# Executable
#===============================================================================
#if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
#    qt_add_executable(${CMAKE_PROJECT_NAME} MANUAL_FINALIZATION
#       ${SOURCE_FILES}
#       ${QM_FILES}
#       ${DESKTOP_FILE}
#   )
#else()
    add_executable(${CMAKE_PROJECT_NAME}
        ${SOURCE_FILES}
        ${QM_FILES}
        ${DESKTOP_FILE}
    )
#endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE SDDM_CONF_VERSION="${CMAKE_PROJECT_VERSION}")

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

#if(QT_VERSION_MAJOR EQUAL 6)
#    qt_finalize_executable(${CMAKE_PROJECT_NAME})
#endif()
#===============================================================================
# Install
#===============================================================================
install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES ${DESKTOP_FILE}
    DESTINATION ${CMAKE_INSTALL_DATADIR}/applications/
)
install(FILES resources/${APPSTREAM_ID}.appdata.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo
)
#===============================================================================
# Project information
#===============================================================================
message(STATUS "
Project name:                     ${CMAKE_PROJECT_NAME}
Version:                          ${CMAKE_PROJECT_VERSION}
Build type:                       ${CMAKE_BUILD_TYPE}
Install prefix:                   ${CMAKE_INSTALL_PREFIX}
Update translations before build: ${UPDATE_TRANSLATIONS}

Qt version:                       ${Qt5Core_VERSION_STRING}

Compiler CXX debug flags:         ${CMAKE_CXX_FLAGS_DEBUG}
Compiler CXX release flags:       ${CMAKE_CXX_FLAGS_RELEASE}
Compiler CXX min size flags:      ${CMAKE_CXX_FLAGS_MINSIZEREL}
")
