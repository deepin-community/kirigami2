/*
 *  SPDX-FileCopyrightText: 2016 Aleix Pol Gonzalez <aleixpol@kde.org>
 *  SPDX-FileCopyrightText: 2016 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15

import org.kde.kirigami 2.20 as Kirigami

Kirigami.ApplicationWindow {
    id: main

    pageStack.initialPage: Kirigami.ScrollablePage {
        ListView {
            model: 10
            delegate: Rectangle {
                width: 100
                height: 30
                color: ListView.isCurrentItem ? "red" : "white"
            }
        }
    }
}
