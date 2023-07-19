/*
 *  SPDX-FileCopyrightText: 2016 Aleix Pol Gonzalez <aleixpol@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

import org.kde.kirigami 2.20 as Kirigami

Kirigami.ApplicationWindow {
    id: main

    Component {
        id: keyPage
        Kirigami.Page {
            id: page

            // Don't remove, used in autotests
            readonly property alias lastKey: see.text

            Label {
                id: see
                anchors.centerIn: parent
                color: page.activeFocus ? Kirigami.Theme.focusColor : Kirigami.Theme.textColor
            }

            Keys.onPressed: event => {
                if (event.text) {
                    see.text = event.text
                } else {
                    see.text = event.key
                }
            }

            Keys.onEnterPressed: main.showPassiveNotification("page!")
        }
    }

    header: Label {
        padding: Kirigami.Units.largeSpacing
        text: `focus: ${main.activeFocusItem}, current: ${main.pageStack.currentIndex}`
    }

    Component.onCompleted: {
        main.pageStack.push(keyPage)
        main.pageStack.push(keyPage)
    }
}
