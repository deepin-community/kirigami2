/*
 *  SPDX-FileCopyrightText: 2021 Felipe Kinoshita <kinofhek@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.20 as Kirigami

Kirigami.ScrollablePage {
    title: qsTr("General")

    QQC2.CheckBox {
        Kirigami.FormData.label: i18n("Something")
        text: i18n("Do something")
    }
}
