/*
 *  SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.7
import QtQml 2.15
import QtQuick.Layouts 1.2
import QtQuick.Controls 2.4 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import "private" as P

/**
 * @brief A toolbar built out of a list of actions.
 *
 * The default representation for visible actions is a QtQuick.Controls.ToolButton, but
 * it can be changed by setting the `Action.displayComponent` for an action.
 * The ActionToolBar component will try to display has many actions as possible but
 * The default behavior of ActionToolBar is to display as many actions as possible,
 * placing those that will not fit into an overflow menu. This can be changed by
 * setting the `displayHint` property on an Action. For example, when setting the
 * `DisplayHint.KeepVisible` display hint, ActionToolBar will try to keep that action
 * in view as long as possible, using an icon-only button if a button with text
 * does not fit.
 *
 * @inherit QtQuick.Controls.Control
 * @since 2.5
 */
QQC2.Control {
    id: root

//BEGIN properties
    /**
     * @brief This property holds a list of visible actions.
     *
     * The ActionToolBar will try to display as many actions as possible.
     * Those that won't fit will go into an overflow menu.
     *
     * @property list<Action> actions
     */
    property alias actions: layout.actions

    /**
     * @brief This property holds a list of hidden actions.
     *
     * These actions will always be displayed in the overflow menu, even if there is enough space.
     *
     * @deprecated since 2.14, use the AlwaysHide hint on actions instead.
     * @property list<Action> hiddenActions
     * @since 2.6
     */
    property list<QtObject> hiddenActions
    onHiddenActionsChanged: print("ActionToolBar::hiddenActions is deprecated, use the AlwaysHide hint on your actions instead")

    /**
     * @brief This property holds whether the buttons will have a flat appearance.
     *
     * default: ``true``
     */
    property bool flat: true

    /**
     * @brief This property determines how the icon and text are displayed within the button.
     *
     * Permitted values are:
     * * ``Button.IconOnly``
     * * ``Button.TextOnly``
     * * ``Button.TextBesideIcon``
     * * ``Button.TextUnderIcon``
     *
     * default: ``Controls.Button.TextBesideIcon``
     *
     * @see QtQuick.Controls.AbstractButton
     * @property int display
     */
    property int display: QQC2.Button.TextBesideIcon

    /**
     * @brief This property holds the alignment of the buttons.
     *
     * When there is more space available than required by the visible delegates,
     * we need to determine how to place the delegates.
     *
     * When there is more space available than required by the visible action delegates,
     * we need to determine where to position them.
     *
     * default: ``Qt.AlignRight``
     *
     * @see Qt::AlignmentFlag
     * @property int alignment
     */
    property alias alignment: layout.alignment

    /**
     * @brief This property holds the position of the toolbar.
     *
     * If this ActionToolBar is the contentItem of a QQC2 Toolbar, the position is bound to the ToolBar's position
     *
     * Permitted values are:
     * * ``ToolBar.Header``: The toolbar is at the top, as a window or page header.
     * * ``ToolBar.Footer``: The toolbar is at the bottom, as a window or page footer.
     *
     * @property int position
     */
    property int position: parent && parent.hasOwnProperty("position")
            ? parent.position
            : QQC2.ToolBar.Header

    /**
     * @brief This property holds the maximum width of the content of this ToolBar.
     *
     * If the toolbar's width is larger than this value, empty space will
     * be added on the sides, according to the Alignment property.
     *
     * The value of this property is derived from the ToolBar's actions and their properties.
     *
     * @property int maximumContentWidth
     */
    readonly property alias maximumContentWidth: layout.implicitWidth

    /**
     * @brief This property holds the name of the icon to use for the overflow menu button.
     *
     * default: ``"overflow-menu"``
     *
     * @since 5.65
     * @since 2.12
     */
    property string overflowIconName: "overflow-menu"

    /**
     * @brief This property holds the combined width of all visible delegates.
     * @property int visibleWidth
     */
    property alias visibleWidth: layout.visibleWidth

    /**
     * @brief This property sets the handling method for items that do not match the toolbar's height.
     *
     * When toolbar items do not match the height of the toolbar, there are
     * several ways we can deal with this. This property sets the preferred way.
     *
     * Permitted values are:
     * * ``HeightMode.AlwaysCenter``
     * * ``HeightMode.AlwaysFill``
     * * ``AlwaysFill.ConstrainIfLarger``
     *
     * default: ``HeightMode::ConstrainIfLarger``
     *
     * @see ToolBarLayout::heightMode
     * @see ToolBarLayout::HeightMode
     * @property ToolBarLayout::HeightMode heightMode
     */
    property alias heightMode: layout.heightMode
//END properties

    implicitHeight: layout.implicitHeight
    implicitWidth: layout.implicitWidth

    Layout.minimumWidth: layout.minimumWidth
    Layout.preferredWidth: 0
    Layout.fillWidth: true

    leftPadding: 0
    rightPadding: 0
    topPadding: 0
    bottomPadding: 0

    contentItem: Kirigami.ToolBarLayout {
        id: layout
        spacing: Kirigami.Units.smallSpacing
        layoutDirection: root.LayoutMirroring.enabled ? Qt.RightToLeft : Qt.LeftToRight

        fullDelegate: P.PrivateActionToolButton {
            flat: root.flat
            display: root.display
            action: Kirigami.ToolBarLayout.action
        }

        iconDelegate: P.PrivateActionToolButton {
            flat: root.flat
            display: QQC2.Button.IconOnly
            action: Kirigami.ToolBarLayout.action

            showMenuArrow: false

            menuActions: {
                if (action.displayComponent) {
                    return [action]
                }

                if (action.hasOwnProperty("children") && action.children.length > 0) {
                    return Array.prototype.map.call(action.children, i => i)
                }

                return []
            }
        }

        moreButton: P.PrivateActionToolButton {
            flat: root.flat

            action: Kirigami.Action {
                tooltip: qsTr("More Actions")
                icon.name: root.overflowIconName
                displayHint: Kirigami.DisplayHint.IconOnly | Kirigami.DisplayHint.HideChildIndicator
            }

            menuActions: {
                if (root.hiddenActions.length === 0) {
                    return root.actions
                } else {
                    const result = []
                    result.concat(Array.prototype.map.call(root.actions, i => i))
                    result.concat(Array.prototype.map.call(hiddenActions, i => i))
                    return result
                }
            }

            menuComponent: P.ActionsMenu {
                submenuComponent: P.ActionsMenu {
                    Binding {
                        target: parentItem
                        property: "visible"
                        value: layout.hiddenActions.includes(parentAction)
                               && (parentAction.visible === undefined || parentAction.visible)
                        restoreMode: Binding.RestoreBinding
                    }
                }

                itemDelegate: P.ActionMenuItem {
                    visible: layout.hiddenActions.includes(action)
                             && (action.visible === undefined || action.visible)
                }

                loaderDelegate: Loader {
                    property var action
                    height: visible ? implicitHeight : 0
                    visible: layout.hiddenActions.includes(action)
                             && (action.visible === undefined || action.visible)
                }

                separatorDelegate: QQC2.MenuSeparator {
                    property var action
                    visible: layout.hiddenActions.includes(action)
                             && (action.visible === undefined || action.visible)
                }
            }
        }
    }
}
