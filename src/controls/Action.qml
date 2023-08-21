/*
 *  SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.7
import QtQuick.Controls 2.4 as QQC2
import org.kde.kirigami 2.14 as Kirigami

/**
 * @brief An item that represents an abstract Action
 * @inherit QtQuick.QQC2.Action
 */
QQC2.Action {
    id: root

//BEGIN properties
    /**
     * @brief This property holds whether the graphic representation of the action
     * is supposed to be visible.
     *
     * It's up to the action representation to honor this property.
     *
     * default: ``true``
     */
    property bool visible: true

    /**
     * @brief This property holds the icon name for the action. This will pick the icon with the given name from the current theme.
     * @deprecated Use icon.name instead.
     * @property string iconName
     */
    property alias iconName: root.icon.name

    /**
     * @brief This property holds an url to an icon file or resource url for the action.
     * @note Use this if you want a specific file rather than an icon from the theme.
     * @deprecated Use icon.name instead.
     * @property url iconSource
     */
    property alias iconSource: root.icon.source

    /**
     * @brief This property holds the tooltip text that is shown when the cursor is hovering over the control.
     *
     * Leaving this undefined or setting it to an empty string means that no tooltip will be shown when
     * the cursor is hovering over the control that triggers the tooltip.
     * @warning Tooltips may not be supported on all platforms.
     */
    property string tooltip

    /**
     * @brief This property sets whether this action is a separator action.
     *
     * default: ``false``
     */
    property bool separator: false

    /**
     * @brief This property sets whether this action  becomes a title displaying
     * its child actions as sub-items in GlobalDrawers and ContextDrawers.
     *
     * default: ``false``
     *
     * @since 2.6
     */
    property bool expandible: false

    /**
     * @brief This property holds the parent action.
     */
    property QQC2.Action parent

    /**
     * @brief This property sets this action's display type.
     *
     * These are provided to implementations to indicate a preference for certain display
     * styles.
     *
     * default: ``Kirigami.DisplayHint.NoPreference``
     *
     * @note This property contains only preferences, implementations may choose to disregard them.
     * @see org::kde::kirigami::DisplayHint
     * @since 2.12
     */
    property int displayHint: Kirigami.DisplayHint.NoPreference

    /**
     * @brief This is a helper function to check if a certain display hint has been set.
     *
     * This function is mostly convenience to enforce the mutual exclusivity of KeepVisible and AlwaysHide.
     *
     * @param hint The display hint to check if it is set.
     * @see org::kde::kirigami::DisplayHint
     * @deprecated since 2.14, Use DisplayHint.displayHintSet(action, hint) instead.
     * @return true if the hint was set for this action, false if not.
     * @since 2.12
     */
    function displayHintSet(hint) {
        print("Action::displayHintSet is deprecated, use DisplayHint.displayHintSet(action, hint)")
        return Kirigami.DisplayHint.displayHintSet(root, hint);
    }

    /**
     * @brief This property holds the component that should be used for displaying this action.
     * @note This can be used to display custom components in the toolbar.
     * @since 5.65
     * @since 2.12
     */
    property Component displayComponent: null

    /**
     * @brief This property holds a list of child actions.
     *
     * This is useful for tree-like menus, such as the GlobalDrawer.
     *
     * Example usage:
     * @code
     * Action {
     *    text: "Tools"
     *    Action {
     *        text: "Action1"
     *    }
     *    Action {
     *        text: "Action2"
     *    }
     * }
     * @endcode
     * @property list<Action> children
     */
    default property list<QtObject> children
//END properties

    onChildrenChanged: {
        let child;
        for (const i in children) {
            child = children[i];
            if (child.hasOwnProperty("parent")) {
                child.parent = root
            }
        }
    }

    /**
     * @brief This property holds the action's visible child actions.
     * @property list<Action> visibleChildren
     */
    readonly property var visibleChildren: {
        const visible = [];
        for (const i in children) {
            const child = children[i];
            if (!child.hasOwnProperty("visible") || child.visible) {
                visible.push(child);
            }
        }
        return visible;
    }

    /**
     * @brief Hints for implementations using Actions indicating preferences about how to display the action.
     * @see org::kde::kirigami::DisplayHint
     * @deprecated since 2.14, use Kirigami.DisplayHint instead.
     */
    enum DisplayHint {
        /**
         * Indicates there is no specific preference.
         */
        NoPreference = 0,
        /**
         * Only display an icon for this Action.
         */
        IconOnly = 1,
        /**
         * Try to keep the action visible even when space constrained.
         * Mutually exclusive with AlwaysHide, KeepVisible has priority.
         */
        KeepVisible = 2,
        /**
         * If possible, hide the action in an overflow menu or similar location.
         * Mutually exclusive with KeepVisible, KeepVisible has priority.
         */
        AlwaysHide = 4,
        /**
         * When this action has children, do not display any indicator (like a
         * menu arrow) for this action.
         */
        HideChildIndicator = 8
    }
}
