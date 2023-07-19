/*
 *  SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami
import QtQuick.Controls 2.15 as QQC2

Kirigami.ApplicationWindow {
    id: root

    width: Kirigami.Units.gridUnit * 60
    height: Kirigami.Units.gridUnit * 40

    pageStack.initialPage: mainPageComponent
    globalDrawer: Kirigami.OverlayDrawer {
        drawerOpen: true
        modal: false
        contentItem: Item {
            implicitWidth: Kirigami.Units.gridUnit * 10

            QQC2.Label {
                text: "This is a sidebar"
                width: parent.width - Kirigami.Units.smallSpacing * 2
                wrapMode: Text.WordWrap
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    //Main app content
    Component {
        id: mainPageComponent
        MultipleColumnsGallery {}
    }
}
