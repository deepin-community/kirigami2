/*
 *  SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.12
import QtQml 2.15
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Window 2.5
import org.kde.kirigami 2.4 as Kirigami
import "templates/private" as TP
/**
 * A window that provides some basic features needed for all apps
 * Use this class only if you need a custom content for your application,
 * different from the Page Row behavior recommended by the HIG and provided
 * by ApplicationWindow.
 * It is recommended to use ApplicationWindow instead
 * @see ApplicationWindow
 *
 * It's usually used as a root QML component for the application.
 * It provides support for a central page stack, side drawers and
 * a top ApplicationHeader, as well as basic support for the
 * Android back button
 *
 * Setting a width and height property on the ApplicationWindow
 * will set its initial size, but it won't set it as an automatically binding.
 * to resize programmatically the ApplicationWindow they need to
 * be assigned again in an imperative fashion
 *
 *
 * Example usage:
 * @code
 * import org.kde.kirigami 2.4 as Kirigami
 *
 * Kirigami.ApplicationWindow {
 *  [...]
 *     globalDrawer: Kirigami.GlobalDrawer {
 *         actions: [
 *            Kirigami.Action {
 *                text: "View"
 *                icon.name: "view-list-icons"
 *                Kirigami.Action {
 *                        text: "action 1"
 *                }
 *                Kirigami.Action {
 *                        text: "action 2"
 *                }
 *                Kirigami.Action {
 *                        text: "action 3"
 *                }
 *            },
 *            Kirigami.Action {
 *                text: "Sync"
 *                icon.name: "folder-sync"
 *            }
 *         ]
 *     }
 *
 *     contextDrawer: Kirigami.ContextDrawer {
 *         id: contextDrawer
 *     }
 *
 *     pageStack: PageStack {
 *         ...
 *     }
 *  [...]
 * }
 * @endcode
 *
 * @inherit QtQuick.Controls.ApplicationWindow
 */
QQC2.ApplicationWindow {
    id: root

//BEGIN properties
    /**
     * @brief This property holds the stack used to allocate the pages and to manage the
     * transitions between them.
     *
     * Put a container here, such as QtQuick.Controls.StackView.
     */
    property Item pageStack

    /**
     * @brief This property sets whether the standard chrome of the app is visible.
     *
     * These are the action button, the drawer handles, and the application header.
     *
     * default: ``true``
     */
    property bool controlsVisible: true

    /**
     * @brief This property holds the drawer for global actions.
     *
     * This drawer can be opened by sliding from the left screen edge
     * or by dragging the ActionButton to the right.
     *
     * @note It is recommended to use the GlobalDrawer here.
     * @property org::kde::kirigami::OverlayDrawer globalDrawer
     */
    property OverlayDrawer globalDrawer

    /**
     * @brief This property tells whether the application is in "widescreen" mode.
     *
     * This is enabled on desktops or horizontal tablets.
     *
     * @note Different styles can have their own logic for deciding this.
     */
    property bool wideScreen: width >= Kirigami.Units.gridUnit * 60

    /**
     * @brief This property holds the drawer for context-dependent actions.
     *
     * The drawer that will be opened by sliding from the right screen edge
     * or by dragging the ActionButton to the left.
     *
     * @note It is recommended to use the ContextDrawer class here.
     *
     * The contents of the context drawer should depend from what page is
     * loaded in the main pageStack
     *
     * Example usage:
     * @code
     * import org.kde.kirigami 2.4 as Kirigami
     *
     * Kirigami.ApplicationWindow {
     *  [...]
     *     contextDrawer: Kirigami.ContextDrawer {
     *         id: contextDrawer
     *     }
     *  [...]
     * }
     * @endcode
     *
     * @code
     * import org.kde.kirigami 2.4 as Kirigami
     *
     * Kirigami.Page {
     *   [...]
     *     contextualActions: [
     *         Kirigami.Action {
     *             icon.name: "edit"
     *             text: "Action text"
     *             onTriggered: {
     *                 // do stuff
     *             }
     *         },
     *         Kirigami.Action {
     *             icon.name: "edit"
     *             text: "Action text"
     *             onTriggered: {
     *                 // do stuff
     *             }
     *         }
     *     ]
     *   [...]
     * }
     * @endcode
     *
     * When this page will be the current one, the context drawer will visualize
     * contextualActions defined as property in that page.
     * @property org::kde::kirigami::ContextDrawer contextDrawer
     */
    property OverlayDrawer contextDrawer

    /**
     * @brief This property tells whether the application is in reachable mode for single hand use.
     *
     * The whole content of the application is moved down the screen to be
     * reachable with the thumb. If wideScreen is true, or reachableModeEnabled is false,
     * this property has no effect.
     *
     * default: ``false``
     */
    property bool reachableMode: false

    /**
     * @brief This property sets whether the application will go into reachable mode on pull down.
     */
    property bool reachableModeEnabled: true

    /**
     * This property holds a standard action that will quit the application when triggered.
     * Its properties have the following values:
     *
     * @code
     * Action {
     *     text: "Quit"
     *     icon.name: "application-exit-symbolic";
     *     shortcut: StandardKey.Quit
     *     [...]
     * @endcode
     * @since 5.76
     */
    readonly property Action quitAction: _quitAction
//END properties

//BEGIN functions
    /**
     * @brief This function shows a little passive notification at the bottom of the app window
     * lasting for few seconds, with an optional action button.
     *
     * @param message The text message to be shown to the user.
     * @param timeout How long to show the message:
     *            possible values: "short", "long" or the number of milliseconds
     * @param actionText Text in the action button, if any.
     * @param callBack A JavaScript function that will be executed when the
     *            user clicks the button.
     */
    function showPassiveNotification(message, timeout, actionText, callBack) {
        notificationsObject.showNotification(message, timeout, actionText, callBack);
    }

   /**
    * @brief This function hides the passive notification at specified index, if any is shown.
    * @param index Index of the notification to hide. Default is 0 (oldest notification).
    */
    function hidePassiveNotification(index = 0) {
        notificationsObject.hideNotification(index);
    }

    /**
     * @brief This function returns application window's object anywhere in the application.
     * @returns a pointer to this application window
     * can be used anywhere in the application.
     */
    function applicationWindow() {
        return root;
    }
//END functions

    LayoutMirroring.enabled: Qt.application.layoutDirection === Qt.RightToLeft
    LayoutMirroring.childrenInherit: true

    color: Kirigami.Theme.backgroundColor

    TP.PassiveNotificationsManager {
        id: notificationsObject
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        z: 1
    }

    MouseArea {
        parent: contentItem.parent
        z: 0
        anchors.fill: parent
        onClicked: mouse => {
            root.reachableMode = false;
        }
        visible: root.reachableMode && root.reachableModeEnabled
        Rectangle {
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.3)
            opacity: 0.15
            Kirigami.Icon {
                anchors.horizontalCenter: parent.horizontalCenter
                y: x
                width: Kirigami.Units.iconSizes.large
                height: width
                source: "go-up"
            }
        }
    }

    contentItem.z: 1
    contentItem.anchors.left: contentItem.parent.left
    contentItem.anchors.right: contentItem.parent.right
    contentItem.anchors.topMargin: root.wideScreen && header && controlsVisible ? header.height : 0
    contentItem.anchors.leftMargin: root.globalDrawer && root.globalDrawer.modal === false && (!root.pageStack || root.pageStack.leftSidebar !== root.globalDrawer) ? root.globalDrawer.width * root.globalDrawer.position : 0
    contentItem.anchors.rightMargin: root.contextDrawer && root.contextDrawer.modal === false ? root.contextDrawer.width * root.contextDrawer.position : 0

    Binding {
        when: menuBar !== undefined
        target: menuBar
        property: "x"
        value: -contentItem.x
        restoreMode: Binding.RestoreBinding
    }
    Binding {
        when: header !== undefined
        target: header
        property: "x"
        value: -contentItem.x
        restoreMode: Binding.RestoreBinding
    }
    Binding {
        when: footer !== undefined
        target: footer
        property: "x"
        value: -contentItem.x
        restoreMode: Binding.RestoreBinding
    }

    contentItem.transform: Translate {
        Behavior on y {
            NumberAnimation {
                duration: Kirigami.Units.longDuration
                easing.type: Easing.InOutQuad
            }
        }
        y: root.reachableMode && root.reachableModeEnabled && !root.wideScreen ? root.height/2 : 0
        x: root.globalDrawer && root.globalDrawer.modal === true && root.globalDrawer.toString().indexOf("SplitDrawer") === 0 ? root.globalDrawer.contentItem.width * root.globalDrawer.position : 0
    }
    //Don't want overscroll in landscape mode
    onWidthChanged: {
        if (width > height) {
            root.reachableMode = false;
        }
    }
    Binding {
        when: globalDrawer !== undefined && root.visible
        target: globalDrawer
        property: "parent"
        value: overlay
        restoreMode: Binding.RestoreBinding
    }
    Binding {
        when: contextDrawer !== undefined && root.visible
        target: contextDrawer
        property: "parent"
        value: overlay
        restoreMode: Binding.RestoreBinding
    }
    onPageStackChanged: pageStack.parent = contentItem;

    width: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 30 : Kirigami.Units.gridUnit * 55
    height: Kirigami.Settings.isMobile ? Kirigami.Units.gridUnit * 45 : Kirigami.Units.gridUnit * 40
    visible: true

    Component.onCompleted: {
        // Explicitly break the binding as we need this to be set only at startup.
        // if the bindings are active, after this the window is resized by the
        // compositor and then the bindings are reevaluated, then the window
        // size would reset ignoring what the compositor asked.
        // see BUG 433849
        root.width = root.width;
        root.height = root.height;
    }

    Action {
        id: _quitAction
        text: qsTr("Quit")
        icon.name: "application-exit";
        shortcut: StandardKey.Quit
        onTriggered: source => root.close()
    }
    Shortcut {
        sequence: _quitAction.shortcut
        context: Qt.ApplicationShortcut
        onActivated: root.close()
    }
}
