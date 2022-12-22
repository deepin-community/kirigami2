/*
 * Copyright 2021 Devin Lin <espidev@gmail.com>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQml 2.15
import QtQuick.Templates 2.15 as T
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import QtGraphicalEffects 1.12

/**
 * Page navigation tab-bar, used as an alternative to sidebars for 3-5 elements.
 * 
 * Can be combined with secondary toolbars above (if in the footer) to provide page actions.
 * 
 * Example:
 * @code{.qml}
 * import org.kde.kirigami 2.19 as Kirigami
 * 
 * import QtQuick 2.15
 * import QtQuick.Controls 2.15
 * import QtQuick.Layouts 1.15
 * import org.kde.kirigami 2.19 as Kirigami
 * 
 * Kirigami.ApplicationWindow {
 *     title: "Clock"
 * 
 *     pageStack.initialPage: worldPage
 *     Kirigami.Page {
 *         id: worldPage
 *         title: "World"
 *         visible: false
 *     }
 *     Kirigami.Page {
 *         id: timersPage
 *         title: "Timers"
 *         visible: false
 *     }
 *     Kirigami.Page {
 *         id: stopwatchPage
 *         title: "Stopwatch"
 *         visible: false
 *     }
 *     Kirigami.Page {
 *         id: alarmsPage
 *         title: "Alarms"
 *         visible: false
 *     }
 *     
 *     
 *     footer: Kirigami.NavigationTabBar {
 *         actions: [
 *             Kirigami.Action {
 *                 iconName: "globe"
 *                 text: "World"
 *                 checked: worldPage.visible
 *                 onTriggered: {
 *                      if (!worldPage.visible) {
 *                          while (pageStack.depth > 0) {
 *                              pageStack.pop();
 *                          }
 *                          pageStack.push(worldPage);
 *                     }
 *                 }
 *             },
 *             Kirigami.Action {
 *                 iconName: "player-time"
 *                 text: "Timers"
 *                 checked: timersPage.visible
 *                 onTriggered: {
 *                     if (!timersPage.visible) {
 *                         while (pageStack.depth > 0) {
 *                             pageStack.pop();
 *                         }
 *                         pageStack.push(timersPage);
 *                     }
 *                 }
 *             },
 *             Kirigami.Action {
 *                 iconName: "chronometer"
 *                 text: "Stopwatch"
 *                 checked: stopwatchPage.visible
 *                 onTriggered: {
 *                     if (!stopwatchPage.visible) {
 *                         while (pageStack.depth > 0) {
 *                             pageStack.pop();
 *                         }
 *                         pageStack.push(stopwatchPage);
 *                     }
 *                 }
 *             },
 *             Kirigami.Action {
 *                 iconName: "notifications"
 *                 text: "Alarms"
 *                 checked: alarmsPage.visible
 *                 onTriggered: {
 *                     if (!alarmsPage.visible) {
 *                         while (pageStack.depth > 0) {
 *                             pageStack.pop();
 *                         }
 *                         pageStack.push(alarmsPage);
 *                     }
 *                 }
 *             }
 *         ]
 *     }
 * }
 *
 * @endcode
 * 
 * @inherit QtQuick.Templates.Toolbar
 * @since 5.87
 * @since org.kde.kirigami 2.19
 */

T.ToolBar {
    id: root

    /**
     * This property holds the list of actions in the toolbar.
     */
    property list<Kirigami.Action> actions

    /**
     * The property holds the maximum width of the toolbar actions, before margins are added.
     */
    property real maximumContentWidth: {
        let minDelegateWidth = Kirigami.Units.gridUnit * 5;
        // always have at least the width of 5 items (so small amounts of actions look natural)
        return Math.max(minDelegateWidth * actions.length, minDelegateWidth * 5);
    }

    /**
     * This property holds the background color of the toolbar.
     */
    property color backgroundColor: Kirigami.Theme.backgroundColor

    /**
     * This property holds the foreground color of the toolbar (text, icon).
     */
    property color foregroundColor: Qt.rgba(Kirigami.Theme.textColor.r, Kirigami.Theme.textColor.g, Kirigami.Theme.textColor.b, 0.7)

    /**
     * This property holds the highlight foreground color (text, icon when action is checked).
     */
    property color highlightForegroundColor: Kirigami.ColorUtils.tintWithAlpha(Kirigami.Theme.highlightColor, Kirigami.Theme.textColor, 0.5)

    /**
     * This property holds the color of the highlight bar, when an action is checked.
     */
    property color highlightBarColor: Kirigami.Theme.highlightColor

    /**
     * This property holds whether the toolbar should provide its own shadow.
     */
    property bool shadow: true

    /**
     * This property holds the index of the checked tab.
     * 
     * 
     * If the index set is out of bounds, or the triggered signal did not change any checked property of an action, the index
     * will remain the same.
     */
    property int currentIndex: tabGroup.checkedButton && tabGroup.buttons.length > 0 ? tabGroup.checkedButton.tabIndex : -1

    /**
     * This property holds the number of tab buttons.
     */
    readonly property int count: tabGroup.buttons.length

    /**
     * This property holds the ButtonGroup used to manage the tabs.
     */
    readonly property T.ButtonGroup tabGroup: tabGroup

    onCurrentIndexChanged: {
        if (currentIndex === -1) {
            if (tabGroup.checkState !== Qt.Unchecked) {
                tabGroup.checkState = Qt.Unchecked;
            }
            return;
        }
        if (tabGroup.checkedButton.tabIndex !== currentIndex) {
            const buttonForCurrentIndex = tabGroup.buttons[currentIndex]
            if (buttonForCurrentIndex.action) {
                // trigger also toggles and causes clicked() to be emitted
                buttonForCurrentIndex.action.trigger();
            } else {
                // toggle() does not trigger the action,
                // so don't use it if you want to use an action.
                // It also doesn't cause clicked() to be emitted.
                buttonForCurrentIndex.toggle();
            }
        }
    }

    // Using Math.round() on horizontalPadding can cause the contentItem to jitter left and right when resizing the window.
    horizontalPadding: Math.floor(Math.max(0, width - root.maximumContentWidth) / 2)
    contentWidth: Math.ceil(Math.min(root.availableWidth, root.maximumContentWidth))
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset, contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset, contentHeight + topPadding + bottomPadding)

    Kirigami.Theme.colorSet: Kirigami.Theme.Window

    background: Rectangle { // color & shadow
        implicitHeight: Kirigami.Units.gridUnit * 3 + Kirigami.Units.smallSpacing * 2
        color: root.backgroundColor
        RectangularGlow {
            anchors.fill: parent
            z: -1
            visible: root.shadow
            glowRadius: 5
            spread: 0.3
            color: Qt.rgba(0.0, 0.0, 0.0, 0.15)
        }
    }

    // Using Row because setting just width is more convenient than having to set Layout.minimumWidth and Layout.maximumWidth
    contentItem: Row {
        id: rowLayout
        spacing: root.spacing
    }

    // Used to manage which tab is checked and change the currentIndex
    T.ButtonGroup {
        id: tabGroup
        exclusive: true
        buttons: root.contentItem.children
        
        onCheckedButtonChanged: {
            if (root.currentIndex !== checkedButton.tabIndex) {
                root.currentIndex = checkedButton.tabIndex;
            }
        }
    }

    // Using an Instantiator instead of a Repeater allows us to use parent.visibleChildren.length without including a Repeater in that count.
    Instantiator {
        id: instantiator
        model: root.actions
        delegate: NavigationTabButton {
            id: delegate
            parent: root.contentItem
            action: modelData
            visible: modelData.visible
            T.ButtonGroup.group: tabGroup
            // Workaround setting the action when checkable is not explicitly set making tabs uncheckable
            onActionChanged: action.checkable = true
            
            foregroundColor: root.foregroundColor
            highlightForegroundColor: root.highlightForegroundColor
            highlightBarColor: root.highlightBarColor
        }
    }
}


