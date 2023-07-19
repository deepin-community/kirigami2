/*
 *  SPDX-FileCopyrightText: 2022 Aleix Pol <aleixpol@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2

import org.kde.kirigami 2.20 as Kirigami

Kirigami.ApplicationWindow {
    height: 720
    width: 360
    visible: true

    Kirigami.OverlaySheet {
        id: sheet

        title: "Certificate Viewer"

        ColumnLayout {
            QQC2.DialogButtonBox {
                Layout.fillWidth: true

                QQC2.Button {
                    QQC2.DialogButtonBox.buttonRole: QQC2.DialogButtonBox.ActionRole
                    text: "Export…"
                }

                QQC2.Button {
                    QQC2.DialogButtonBox.buttonRole: QQC2.DialogButtonBox.DestructiveRole
                    text: "Close"
                    icon.name: "dialog-close"
                }
            }
        }
    }

    Timer {
        interval: 150
        running: true
        onTriggered: sheet.open()
    }
}
