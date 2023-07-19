set(KIRIGAMI_INSTALL_INCLUDEDIR "${KDE_INSTALL_INCLUDEDIR_KF}/Kirigami2")

add_library(KF5Kirigami2 ${libkirigami_SRCS})
add_library(KF5::Kirigami2 ALIAS KF5Kirigami2)

target_sources(KF5Kirigami2 PRIVATE
    platformtheme.cpp
    basictheme.cpp
    kirigamipluginfactory.cpp
    tabletmodewatcher.cpp
    styleselector.cpp
    units.cpp
    virtualkeyboardwatcher.cpp
)

set(libkirigami_extra_sources "")

#use dbus on linux, bsd etc, but not android and apple stuff
if (UNIX AND NOT ANDROID AND NOT(APPLE) AND NOT(DISABLE_DBUS))
    qt_add_dbus_interface(libkirigami_extra_sources org.kde.KWin.TabletModeManager.xml tabletmodemanager_interface)
    qt_add_dbus_interface(libkirigami_extra_sources org.kde.KWin.VirtualKeyboard.xml virtualkeyboard_interface)
    set(LIBKIRIGAMKI_EXTRA_LIBS Qt${QT_MAJOR_VERSION}::DBus)
endif()

ecm_qt_declare_logging_category(libkirigami_extra_sources
    HEADER loggingcategory.h
    IDENTIFIER KirigamiLog
    CATEGORY_NAME kf.kirigami
    DEFAULT_SEVERITY Warning
)

ecm_generate_export_header(KF5Kirigami2
    VERSION ${KF_VERSION}
    BASE_NAME Kirigami2
    DEPRECATION_VERSIONS 5.80 5.86
)

target_sources(KF5Kirigami2 PRIVATE ${libkirigami_extra_sources})

target_include_directories(KF5Kirigami2
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
    INTERFACE "$<INSTALL_INTERFACE:${KIRIGAMI_INSTALL_INCLUDEDIR}>"
)

target_link_libraries(KF5Kirigami2
    PUBLIC
        Qt${QT_MAJOR_VERSION}::Core
    PRIVATE
        Qt${QT_MAJOR_VERSION}::Qml
        Qt${QT_MAJOR_VERSION}::Quick
        Qt${QT_MAJOR_VERSION}::QuickControls2
        ${LIBKIRIGAMKI_EXTRA_LIBS}
)

set_target_properties(KF5Kirigami2 PROPERTIES
    VERSION     ${KIRIGAMI2_VERSION}
    SOVERSION   ${KIRIGAMI2_SOVERSION}
    EXPORT_NAME "Kirigami2"
)

ecm_generate_headers(Kirigami2_CamelCase_HEADERS
    HEADER_NAMES
    PlatformTheme
    KirigamiPluginFactory
    TabletModeWatcher
    Units
    VirtualKeyboardWatcher

    PREFIX Kirigami
    REQUIRED_HEADERS Kirigami2_HEADERS
)

if(NOT BUILD_SHARED_LIBS)
    ecm_generate_headers(Kirigami2_HEADERS
        HEADER_NAMES KirigamiPlugin
        REQUIRED_HEADERS Kirigami2_HEADERS
        RELATIVE ..
    )
endif()

install(TARGETS KF5Kirigami2
        EXPORT KF5Kirigami2Targets
        ${KF_INSTALL_TARGETS_DEFAULT_ARGS})

if(BUILD_SHARED_LIBS)
    list(APPEND Kirigami2_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/kirigami2_export.h)
endif()

install(FILES ${Kirigami2_HEADERS}
        DESTINATION ${KIRIGAMI_INSTALL_INCLUDEDIR}/kirigami # prefix matching C++ namespace
        COMPONENT Devel)
install(FILES ${Kirigami2_CamelCase_HEADERS}
        DESTINATION ${KIRIGAMI_INSTALL_INCLUDEDIR}/Kirigami # prefix matching C++ namespace
        COMPONENT Devel)

# provide compat headers for old broken C++ namespaceless include path
if(NOT EXCLUDE_DEPRECATED_BEFORE_AND_AT STREQUAL "CURRENT" AND
   EXCLUDE_DEPRECATED_BEFORE_AND_AT VERSION_LESS 5.91.0)

    function(generate_compat_headers)
        foreach(classname ${ARGV})
            string(TOLOWER ${classname} classname_lc)
            set(HEADER_NAME "${classname_lc}.h")
            # normal header
            set(compat_header "${CMAKE_CURRENT_BINARY_DIR}/compat/${HEADER_NAME}")
            set(NEW_INCLUDE "kirigami/${HEADER_NAME}")
            configure_file(compatheader.h.in ${compat_header} @ONLY)
            install(FILES ${compat_header}
                DESTINATION ${KIRIGAMI_INSTALL_INCLUDEDIR}
                COMPONENT Devel
            )
            # CamelCase header
            set(compat_header "${CMAKE_CURRENT_BINARY_DIR}/compat/${classname}")
            set(NEW_INCLUDE "Kirigami/${classname}")
            configure_file(compatheader.h.in ${compat_header} @ONLY)
            install(FILES ${compat_header}
                DESTINATION ${KIRIGAMI_INSTALL_INCLUDEDIR}
                COMPONENT Devel
            )
        endforeach()
    endfunction()

    generate_compat_headers(
        PlatformTheme
        KirigamiPluginFactory
        TabletModeWatcher
        Units
    )
endif()

if(BUILD_QCH)
    ecm_add_qch(
        KF5Kirigami2_QCH
        NAME Kirigami2
        BASE_NAME KF5Kirigami2
        VERSION ${KF_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${Kirigami2_HEADERS}
        MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        LINK_QCHS
            Qt5Core_QCH
        BLANK_MACROS
            KIRIGAMI_EXPORT
            KIRIGAMI_DEPRECATED
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()

include(ECMGeneratePriFile)
ecm_generate_pri_file(BASE_NAME Kirigami2
    LIB_NAME KF5Kirigami2
    DEPS "core"
    FILENAME_VAR PRI_FILENAME
    INCLUDE_INSTALL_DIR ${KIRIGAMI_INSTALL_INCLUDEDIR}
)
install(FILES ${PRI_FILENAME}
        DESTINATION ${ECM_MKSPECS_INSTALL_DIR})

