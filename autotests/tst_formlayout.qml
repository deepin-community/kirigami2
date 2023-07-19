/*
 *  SPDX-FileCopyrightText: 2022 Connor Carney <hello@connorcarney.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.1
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.19 as Kirigami
import QtTest 1.0

TestCase {
    id: testCase
    name: "FormLayout"

    Component {
        id: fractionalSizeRoundingComponent
        Window {
            property var item: fractionalSizeItem
            width: 600
            height: 400
            Kirigami.FormLayout {
                anchors.fill: parent
                Item {
                    id: fractionalSizeItem
                    implicitWidth: 160.375
                    implicitHeight: 17.001
                    Layout.fillWidth: true
                }
            }
        }
    }

    function test_fractional_width_rounding() {
        let window = fractionalSizeRoundingComponent.createObject();
        let item = window.item;
        window.show();

        verify(item.width >= item.implicitWidth, "implicit width should not be rounded down");
        fuzzyCompare(item.width, item.implicitWidth, 1);

        window.close();
    }

    function test_fractional_height_rounding() {
        let window = fractionalSizeRoundingComponent.createObject();
        let item = window.item;
        window.show();

        verify(item.height >= item.implicitHeight, "implicit height should not be rounded down");
        fuzzyCompare(item.height, item.implicitHeight, 1);

        window.close();
    }
}
