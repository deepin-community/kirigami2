
add_subdirectory(libkirigami)

ecm_add_qml_module(KirigamiPlugin URI "org.kde.kirigami" VERSION 2.0 CLASSNAME KirigamiPlugin)

ecm_add_qml_module_dependencies(KirigamiPlugin DEPENDS
    "QtQuick.Controls 2.15"
    "QtGraphicalEffects 1.0"
)

ecm_create_qm_loader(kirigami_QM_LOADER libkirigami2plugin_qt)
target_sources(KirigamiPlugin PRIVATE ${kirigami_QM_LOADER})

target_include_directories(KirigamiPlugin PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libkirigami
    ${CMAKE_CURRENT_BINARY_DIR}/libkirigami
)

target_sources(KirigamiPlugin PRIVATE
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
    inputmethod.cpp

    scenegraph/managedtexturenode.cpp
    scenegraph/managedtexturenode.h
    scenegraph/paintedrectangleitem.cpp
    scenegraph/paintedrectangleitem.h
    ${CMAKE_CURRENT_BINARY_DIR}/libkirigami/loggingcategory.cpp

    scenegraph/shaders/shaders.qrc
)
if (HAVE_QTGUI_OPENGL AND NOT KF6_PORTING_TODO)
    target_sources(KirigamiPlugin PRIVATE
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
    )
endif()
add_subdirectory(scenegraph/shaders6)

ecm_target_qml_sources(KirigamiPlugin SOURCES
    controls/Action.qml
    controls/AbstractApplicationHeader.qml
    controls/AbstractApplicationWindow.qml
    controls/AbstractListItem.qml
    controls/ApplicationHeader.qml
    controls/ToolBarApplicationHeader.qml
    controls/ApplicationWindow.qml
    controls/BasicListItem.qml
    controls/OverlayDrawer.qml
    controls/ContextDrawer.qml
    controls/GlobalDrawer.qml
    controls/Heading.qml
    controls/Separator.qml
    controls/PageRow.qml
    controls/Label.qml
    controls/OverlaySheet.qml
    controls/Page.qml
    controls/ScrollablePage.qml
    controls/SwipeListItem.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.1 SOURCES
    controls/AbstractItemViewHeader.qml
    controls/ItemViewHeader.qml
    controls/AbstractApplicationItem.qml
    controls/ApplicationItem.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.3 SOURCES
    controls/FormLayout.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.4 SOURCES
    controls/AbstractCard.qml
    controls/Card.qml
    controls/CardsListView.qml
    controls/CardsGridView.qml
    controls/CardsLayout.qml
    controls/InlineMessage.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.5 SOURCES
    controls/ListItemDragHandle.qml
    controls/ActionToolBar.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.6 SOURCES
    controls/AboutPage.qml
    controls/LinkButton.qml
    controls/UrlButton.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.7 SOURCES
    controls/ActionTextField.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.8 SOURCES
    controls/SearchField.qml
    controls/PasswordField.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.10 SOURCES
    controls/ListSectionHeader.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.11 SOURCES
    controls/PagePoolAction.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.12 SOURCES
    controls/ShadowedImage.qml
    controls/PlaceholderMessage.qml
    controls/RouterWindow.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.13 SOURCES
    controls/Avatar.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.13 PATH swipenavigator SOURCES
    controls/swipenavigator/SwipeNavigator.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.14 SOURCES
    controls/FlexColumn.qml
    controls/CheckableListItem.qml
    controls/Hero.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.17 PATH swipenavigator SOURCES
    controls/swipenavigator/TabViewLayout.qml
    controls/swipenavigator/PageTab.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.18 PATH settingscomponents SOURCES
    controls/settingscomponents/CategorizedSettings.qml
    controls/settingscomponents/SettingAction.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.19 SOURCES
    controls/AboutItem.qml
    controls/NavigationTabBar.qml
    controls/NavigationTabButton.qml
    controls/Dialog.qml
    controls/MenuDialog.qml
    controls/PromptDialog.qml
    controls/AbstractChip.qml
    controls/Chip.qml
    controls/LoadingPlaceholder.qml
)

ecm_target_qml_sources(KirigamiPlugin VERSION 2.20 SOURCES
    controls/SelectableLabel.qml
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH private SOURCES
    controls/private/ActionButton.qml
    controls/private/ActionIconGroup.qml
    controls/private/ActionMenuItem.qml
    controls/private/ActionsMenu.qml
    controls/private/BannerImage.qml
    controls/private/CardsGridViewPrivate.qml
    controls/private/ContextDrawerActionItem.qml
    controls/private/CornerShadow.qml
    controls/private/DefaultCardBackground.qml
    controls/private/DefaultChipBackground.qml
    controls/private/DefaultListItemBackground.qml
    controls/private/DefaultPageTitleDelegate.qml
    controls/private/EdgeShadow.qml
    controls/private/GlobalDrawerActionItem.qml
    controls/private/PageActionPropertyGroup.qml
    controls/private/PrivateActionToolButton.qml
    controls/private/SwipeItemEventFilter.qml
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH private/globaltoolbar SOURCES
    controls/private/globaltoolbar/AbstractPageHeader.qml
    controls/private/globaltoolbar/BreadcrumbControl.qml
    controls/private/globaltoolbar/PageRowGlobalToolBarStyleGroup.qml
    controls/private/globaltoolbar/PageRowGlobalToolBarUI.qml
    controls/private/globaltoolbar/TabBarControl.qml
    controls/private/globaltoolbar/TitlesPageHeader.qml
    controls/private/globaltoolbar/ToolBarPageHeader.qml
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH swipenavigator SOURCES
    controls/swipenavigator/PrivateSwipeHighlight.qml
    controls/swipenavigator/PrivateSwipeProgress.qml
    controls/swipenavigator/PrivateSwipeStack.qml
    controls/swipenavigator/PrivateSwipeTab.qml
    controls/swipenavigator/PrivateSwipeTabBar.qml
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH swipenavigator/templates SOURCES
    controls/swipenavigator/templates/PageTab.qml
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH templates SOURCES
    controls/templates/AbstractApplicationHeader.qml
    controls/templates/AbstractCard.qml
    controls/templates/AbstractChip.qml
    controls/templates/AbstractListItem.qml
    controls/templates/ApplicationHeader.qml
    controls/templates/InlineMessage.qml
    controls/templates/OverlayDrawer.qml
    controls/templates/OverlaySheet.qml
    controls/templates/SingletonHeaderSizeGroup.qml
    controls/templates/SwipeListItem.qml
    controls/templates/qmldir
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH templates/private SOURCES
    controls/templates/private/BackButton.qml
    controls/templates/private/BorderPropertiesGroup.qml
    controls/templates/private/ContextIcon.qml
    controls/templates/private/ForwardButton.qml
    controls/templates/private/GenericDrawerIcon.qml
    controls/templates/private/IconPropertiesGroup.qml
    controls/templates/private/MenuIcon.qml
    controls/templates/private/PassiveNotificationsManager.qml
)

ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH styles/Material SOURCES
    styles/Material/AbstractListItem.qml
    styles/Material/InlineMessage.qml
    styles/Material/Label.qml
    styles/Material/SwipeListItem.qml
    styles/Material/Theme.qml
)

if (DESKTOP_ENABLED)
    ecm_target_qml_sources(KirigamiPlugin PRIVATE PATH styles/org.kde.desktop SOURCES
        styles/org.kde.desktop/AbstractApplicationHeader.qml
        styles/org.kde.desktop/AbstractListItem.qml
        styles/org.kde.desktop/SwipeListItem.qml
        styles/org.kde.desktop/Theme.qml
    )
endif()

target_link_libraries(KirigamiPlugin
    PUBLIC Qt${QT_MAJOR_VERSION}::Core
    PRIVATE
    ${Kirigami_EXTRA_LIBS}
    Qt${QT_MAJOR_VERSION}::GuiPrivate
    Qt${QT_MAJOR_VERSION}::Qml
    Qt${QT_MAJOR_VERSION}::Quick
    Qt${QT_MAJOR_VERSION}::QuickControls2
    Qt${QT_MAJOR_VERSION}::Concurrent
    KF5Kirigami2
)

if (HAVE_OpenMP)
target_link_libraries(KirigamiPlugin PRIVATE OpenMP::OpenMP_CXX)
endif()

ecm_finalize_qml_module(KirigamiPlugin DESTINATION ${KDE_INSTALL_QMLDIR})

ecm_generate_qmltypes(org.kde.kirigami 2.0 DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/kirigami.2)

if (ANDROID)
    install(FILES KF5Kirigami2-android-dependencies.xml
        DESTINATION ${KDE_INSTALL_LIBDIR}
        RENAME KF5Kirigami2_${CMAKE_ANDROID_ARCH_ABI}-android-dependencies.xml
    )
endif()
