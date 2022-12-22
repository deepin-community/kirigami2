if (BUILD_SHARED_LIBS)
    ecm_create_qm_loader(kirigami_QM_LOADER libkirigami2plugin_qt)
else()
    set(KIRIGAMI_STATIC_FILES
        libkirigami/basictheme.cpp
        libkirigami/basictheme_p.h
        libkirigami/kirigamipluginfactory.cpp
        libkirigami/kirigamipluginfactory.h
        libkirigami/platformtheme.cpp
        libkirigami/platformtheme.h
        libkirigami/styleselector.cpp
        libkirigami/styleselector_p.h
        libkirigami/tabletmodewatcher.cpp
        libkirigami/tabletmodewatcher.h
        libkirigami/units.cpp
        libkirigami/units.h
        ${CMAKE_CURRENT_BINARY_DIR}/libkirigami/loggingcategory.cpp)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libkirigami ${CMAKE_CURRENT_BINARY_DIR}/libkirigami)

set(kirigami_SRCS
    avatar.cpp
    avatar.h
    colorutils.cpp
    colorutils.h
    columnview.cpp
    columnview.h
    columnview_p.h
    delegaterecycler.cpp
    delegaterecycler.h
    enums.cpp
    enums.h
    formlayoutattached.cpp
    formlayoutattached.h
    icon.cpp
    icon.h
    imagecolors.cpp
    imagecolors.h
    kirigamiplugin.cpp
    kirigamiplugin.h
    mnemonicattached.cpp
    mnemonicattached.h
    pagepool.cpp
    pagepool.h
    pagerouter.cpp
    pagerouter.h
    scenegraph/managedtexturenode.cpp
    scenegraph/managedtexturenode.h
    scenegraph/paintedrectangleitem.cpp
    scenegraph/paintedrectangleitem.h
    scenegraph/shadowedborderrectanglematerial.cpp
    scenegraph/shadowedborderrectanglematerial.h
    scenegraph/shadowedbordertexturematerial.cpp
    scenegraph/shadowedbordertexturematerial.h
    scenegraph/shadowedrectanglematerial.cpp
    scenegraph/shadowedrectanglematerial.h
    scenegraph/shadowedrectanglenode.cpp
    scenegraph/shadowedrectanglenode.h
    scenegraph/shadowedtexturematerial.cpp
    scenegraph/shadowedtexturematerial.h
    scenegraph/shadowedtexturenode.cpp
    scenegraph/shadowedtexturenode.h
    scenepositionattached.cpp
    scenepositionattached.h
    settings.cpp
    settings.h
    shadowedrectangle.cpp
    shadowedrectangle.h
    shadowedtexture.cpp
    shadowedtexture.h
    sizegroup.cpp
    sizegroup.h
    spellcheckinghint.cpp
    spellcheckinghint.h
    toolbarlayout.cpp
    toolbarlayoutdelegate.cpp
    toolbarlayoutdelegate.h
    toolbarlayout.h
    wheelhandler.cpp
    wheelhandler.h
    ${CMAKE_CURRENT_BINARY_DIR}/libkirigami/loggingcategory.cpp
    ${kirigami_QM_LOADER}
    ${KIRIGAMI_STATIC_FILES}
    )

qt_add_resources(SHADERS scenegraph/shaders/shaders.qrc)

add_subdirectory(libkirigami)

if(NOT BUILD_SHARED_LIBS)
    # `rcc` is a bit dumb and isn't designed to use auto generated files, to
    # avoid polluting the source directory, use absolute paths
    set(kirigami_QML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)

    # First, pre-process the QRC to add the files associated with the right Qt
    # version.
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/../kirigami.qrc.in
        ${CMAKE_CURRENT_BINARY_DIR}/../kirigami.qrc
        @ONLY
    )

    # When using the static library, all QML files need to be shipped within the
    # .a file.
    qt_add_resources(
        RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/../kirigami.qrc
    )

    if (UNIX AND NOT ANDROID AND NOT(APPLE) AND NOT(DISABLE_DBUS))
        qt_add_dbus_interface(kirigami_SRCS libkirigami/org.kde.KWin.TabletModeManager.xml tabletmodemanager_interface)
    endif()
endif()

if (BUILD_SHARED_LIBS)
    add_library(kirigamiplugin ${kirigami_SRCS} ${RESOURCES} ${SHADERS})
else()
    add_library(kirigamiplugin STATIC ${kirigami_SRCS} ${RESOURCES} ${SHADERS})
endif()

if(NOT BUILD_SHARED_LIBS)
    SET_TARGET_PROPERTIES(kirigamiplugin PROPERTIES
        AUTOMOC_MOC_OPTIONS -Muri=org.kde.kirigami)
    if (UNIX AND NOT ANDROID AND NOT(APPLE) AND NOT(DISABLE_DBUS))
        set(Kirigami_EXTRA_LIBS Qt5::DBus)
    else()
        set(Kirigami_EXTRA_LIBS "")
    endif()
else()
    if(WIN32)
        set_target_properties(kirigamiplugin PROPERTIES PREFIX "")
    endif()
    set(Kirigami_EXTRA_LIBS KF5::Kirigami2)
endif()


target_link_libraries(kirigamiplugin
        PUBLIC
            Qt5::Core
        PRIVATE
            ${Kirigami_EXTRA_LIBS} Qt5::GuiPrivate Qt5::Qml Qt5::Quick Qt5::QuickControls2 Qt5::Concurrent
    )

if (BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET kirigamiplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E
                make_directory ${CMAKE_BINARY_DIR}/bin/org/kde/kirigami.2/
        COMMAND ${CMAKE_COMMAND} -E
                copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/controls ${CMAKE_BINARY_DIR}/bin/org/kde/kirigami.2/
        COMMAND ${CMAKE_COMMAND} -E
                copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/styles  ${CMAKE_BINARY_DIR}/bin/org/kde/kirigami.2/styles
        COMMAND ${CMAKE_COMMAND} -E
                copy $<TARGET_FILE:kirigamiplugin> ${CMAKE_BINARY_DIR}/bin/org/kde/kirigami.2/
    )

    install(DIRECTORY controls/ DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2)

    if (DESKTOP_ENABLED)
        install(DIRECTORY styles/org.kde.desktop DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2/styles)
    endif()
    install(DIRECTORY styles/Material DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2/styles)

    install(FILES ${platformspecific} DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2)

    ecm_generate_qmltypes(org.kde.kirigami 2.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2)

    include(ECMGeneratePriFile)
    ecm_generate_pri_file(BASE_NAME Kirigami2 LIB_NAME KF5Kirigami2 DEPS "core qml quick svg" FILENAME_VAR PRI_FILENAME )
    install(FILES ${PRI_FILENAME}
            DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
else()
    target_include_directories(kirigamiplugin
        PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/libkirigami # for loggingcategory.h in the plugin
    )
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2)
endif()

install(TARGETS kirigamiplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2)

if (ANDROID)
    install(FILES KF5Kirigami2-android-dependencies.xml
        DESTINATION ${KDE_INSTALL_LIBDIR}
        RENAME KF5Kirigami2_${CMAKE_ANDROID_ARCH_ABI}-android-dependencies.xml
    )
endif()
