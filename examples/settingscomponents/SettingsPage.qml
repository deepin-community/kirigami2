/*
 *  SPDX-FileCopyrightText: 2021 Felipe Kinoshita <kinofhek@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.18 as Kirigami
import QtQuick.Layouts 1.15

Kirigami.CategorizedSettings {
    actions: [
        Kirigami.SettingAction {
            text: qsTr("General")
            icon.name: "wayland"
            page: Qt.resolvedUrl("./GeneralSettingsPage.qml")
        }
    ]
}
