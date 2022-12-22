/*
 *  SPDX-FileCopyrightText: 2021 Ismael Asensio <isma.af@gmail.com>
 *  SPDX-FileCopyrightText: 2021 David Edmundson <davidedmundson@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.14
import QtQuick.Layouts 1.14
import QtQuick.Controls 2.14 as QQC2
import org.kde.kirigami 2.14 as Kirigami


Rectangle {
    id: background

    implicitWidth: 600
    implicitHeight: 600
    color: Kirigami.Theme.backgroundColor

    Kirigami.FormLayout {
        id: layout
        anchors.centerIn: parent

        QQC2.Button {
            Layout.fillWidth:true
            text: "Open overlay sheet"
            onClicked: sheet.open()
        }
    }

    Kirigami.OverlaySheet {
        id: sheet
        parent: background

        header: QQC2.TextField {
            focus: true
            id: headerText

        }
        footer: QQC2.TextField {
            id: footerText
        }

        ListView {
            id: content
            model: 10

            delegate: Kirigami.BasicListItem {
                label: "Item " + modelData
            }
        }
    }
}
