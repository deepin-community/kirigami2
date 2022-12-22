
/*
 *  SPDX-FileCopyrightText: 2019 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */
import QtQuick 2.15
import org.kde.kirigami 2.15 as Kirigami

Kirigami.ShadowedRectangle {
    id: root

    property bool clickFeedback: false
    property bool hoverFeedback: false
    property color defaultColor: Kirigami.Theme.backgroundColor
    property color pressedColor: Kirigami.ColorUtils.tintWithAlpha(
                                     defaultColor,
                                     Kirigami.Theme.highlightColor, 0.3)
    property color hoverColor: Kirigami.ColorUtils.tintWithAlpha(
                                   defaultColor,
                                   Kirigami.Theme.highlightColor, 0.1)
    property int borderWidth: 1
    property color borderColor: Kirigami.ColorUtils.tintWithAlpha(
                                    color, Kirigami.Theme.textColor, 0.2)

    color: {
        if (clickFeedback && parent.down)
            return root.pressedColor
        else if (hoverFeedback && parent.hovered)
            return root.hoverColor
        return defaultColor
    }
    radius: Kirigami.Units.smallSpacing
    shadow {
        size: Kirigami.Units.largeSpacing
        color: Qt.rgba(0, 0, 0, 0.2)
        yOffset: 2
    }

    border {
        width: borderWidth
        color: borderColor
    }
}
