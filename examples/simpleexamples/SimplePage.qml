/*
 *  SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami

Kirigami.ScrollablePage {
    id: page

    Layout.fillWidth: true
    implicitWidth: Kirigami.Units.gridUnit * (Math.floor(Math.random() * 35) + 8)

    title: i18n("Simple Scrollable Page")

    actions.contextualActions: [
        Kirigami.Action {
            text:"Action for buttons"
            icon.name: "bookmarks"
            onTriggered: print("Action 1 clicked")
        },
        Kirigami.Action {
            text:"Action 2"
            icon.name: "folder"
            enabled: false
        }
    ]

    ColumnLayout {
        width: page.width
        spacing: Kirigami.Units.smallSpacing

        QQC2.Label {
            Layout.fillWidth: true
            wrapMode: Text.WordWrap
            text: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed a sem venenatis, dictum odio vitae, tincidunt sapien. Proin a suscipit ligula, id interdum leo. Donec sed dolor sed lacus dignissim tempor a a lorem. In ullamcorper varius vestibulum. Sed nec arcu semper, varius velit ut, pharetra est. Orci varius natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Integer odio nibh, tincidunt quis condimentum quis, consequat id lacus. Nulla quis mauris erat. Suspendisse rhoncus suscipit massa, at suscipit lorem rhoncus et."
        }
    }
}
