/* SPDX-FileCopyrightText: 2021 Noah Davis <noahadvs@gmail.com>
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.19 as Kirigami
import QtTest 1.15

TestCase {
    id: root
    readonly property real hstep: wheelHandler.horizontalStepSize
    readonly property real vstep: wheelHandler.verticalStepSize
    readonly property real pageWidth: flickable.width - flickable.leftMargin - flickable.rightMargin
    readonly property real pageHeight: flickable.height - flickable.topMargin - flickable.bottomMargin
    readonly property real contentWidth: flickable.contentWidth
    readonly property real contentHeight: flickable.contentHeight
    property alias wheelHandler: wheelHandler
    property alias flickable: flickable

    name: "WheelHandler scrolling"
    visible: true
    when: windowShown
    width: flickable.implicitWidth
    height: flickable.implicitHeight

    function wheelScrolling(angleDelta = 120) {
        let x = flickable.contentX
        let y = flickable.contentY
        const angleDeltaFactor = angleDelta / 120
        mouseWheel(flickable, flickable.leftMargin, 0, -angleDelta, -angleDelta, Qt.NoButton)
        verify(flickable.contentX === Math.round(x + hstep * angleDeltaFactor), "+xTick")
        x = flickable.contentX
        verify(flickable.contentY === Math.round(y + vstep * angleDeltaFactor), "+yTick")
        y = flickable.contentY

        mouseWheel(flickable, flickable.leftMargin, 0, angleDelta, angleDelta, Qt.NoButton)
        verify(flickable.contentX === Math.round(x - hstep * angleDeltaFactor), "-xTick")
        x = flickable.contentX
        verify(flickable.contentY === Math.round(y - vstep * angleDeltaFactor), "-yTick")
        y = flickable.contentY

        if (Qt.platform.pluginName !== "xcb") {
            mouseWheel(flickable, flickable.leftMargin, 0, 0, -angleDelta, Qt.NoButton, Qt.AltModifier)
            verify(flickable.contentX === Math.round(x + hstep * angleDeltaFactor), "+h_yTick")
            x = flickable.contentX
            verify(flickable.contentY === y, "no +yTick")

            mouseWheel(flickable, flickable.leftMargin, 0, 0, angleDelta, Qt.NoButton, Qt.AltModifier)
            verify(flickable.contentX === Math.round(x - hstep * angleDeltaFactor), "-h_yTick")
            x = flickable.contentX
            verify(flickable.contentY === y, "no -yTick")
        }

        mouseWheel(flickable, flickable.leftMargin, 0, -angleDelta, -angleDelta, Qt.NoButton, wheelHandler.pageScrollModifiers)
        verify(flickable.contentX === Math.round(x + pageWidth * angleDeltaFactor), "+xPage")
        x = flickable.contentX
        verify(flickable.contentY === Math.round(y + pageHeight * angleDeltaFactor), "+yPage")
        y = flickable.contentY

        mouseWheel(flickable, flickable.leftMargin, 0, angleDelta, angleDelta, Qt.NoButton, wheelHandler.pageScrollModifiers)
        verify(flickable.contentX === Math.round(x - pageWidth * angleDeltaFactor), "-xPage")
        x = flickable.contentX
        verify(flickable.contentY === Math.round(y - pageHeight * angleDeltaFactor), "-yPage")
        y = flickable.contentY

        if (Qt.platform.pluginName !== "xcb") {
            mouseWheel(flickable, flickable.leftMargin, 0, 0, -angleDelta, Qt.NoButton,
                    Qt.AltModifier | wheelHandler.pageScrollModifiers)
            verify(flickable.contentX === Math.round(x + pageWidth * angleDeltaFactor), "+h_yPage")
            x = flickable.contentX
            verify(flickable.contentY === y, "no +yPage")

            mouseWheel(flickable, flickable.leftMargin, 0, 0, angleDelta, Qt.NoButton,
                    Qt.AltModifier | wheelHandler.pageScrollModifiers)
            verify(flickable.contentX === Math.round(x - pageWidth * angleDeltaFactor), "-h_yPage")
            x = flickable.contentX
            verify(flickable.contentY === y, "no -yPage")
        }
    }

    function test_WheelScrolling() {
        // HID 1bcf:08a0 Mouse
        // Angle delta is 120, like most mice.
        wheelScrolling()
    }

    function test_HiResWheelScrolling() {
        // Logitech MX Master 3
        // Main wheel angle delta is at least 16, plus multiples of 8 when scrolling faster.
        wheelScrolling(16)
    }

    function test_TouchpadScrolling() {
        // UNIW0001:00 093A:0255 Touchpad
        // 2 finger scroll angle delta is at least 3, but larger increments are used when scrolling faster.
        wheelScrolling(3)
    }

    function keyboardScrolling() {
        const originalX = flickable.contentX
        const originalY = flickable.contentY
        let x = originalX
        let y = originalY
        keyClick(Qt.Key_Right)
        verify(flickable.contentX === x + hstep, "Key_Right")
        x = flickable.contentX

        keyClick(Qt.Key_Left)
        verify(flickable.contentX === x - hstep, "Key_Left")
        x = flickable.contentX

        keyClick(Qt.Key_Down)
        verify(flickable.contentY === y + vstep, "Key_Down")
        y = flickable.contentY

        keyClick(Qt.Key_Up)
        verify(flickable.contentY === y - vstep, "Key_Up")
        y = flickable.contentY

        keyClick(Qt.Key_PageDown)
        verify(flickable.contentY === y + pageHeight, "Key_PageDown")
        y = flickable.contentY

        keyClick(Qt.Key_PageUp)
        verify(flickable.contentY === y - pageHeight, "Key_PageUp")
        y = flickable.contentY

        keyClick(Qt.Key_End)
        verify(flickable.contentY === contentHeight - pageHeight, "Key_End")
        y = flickable.contentY

        keyClick(Qt.Key_Home)
        verify(flickable.contentY === originalY, "Key_Home")
        y = flickable.contentY

        keyClick(Qt.Key_PageDown, Qt.AltModifier)
        verify(flickable.contentX === x + pageWidth, "h_Key_PageDown")
        x = flickable.contentX

        keyClick(Qt.Key_PageUp, Qt.AltModifier)
        verify(flickable.contentX === x - pageWidth, "h_Key_PageUp")
        x = flickable.contentX

        keyClick(Qt.Key_End, Qt.AltModifier)
        verify(flickable.contentX === contentWidth - pageWidth, "h_Key_End")
        x = flickable.contentX

        keyClick(Qt.Key_Home, Qt.AltModifier)
        verify(flickable.contentX === originalX, "h_Key_Home")
    }

    function test_KeyboardScrolling() {
        keyboardScrolling()
    }

    function test_StepSize() {
        // 101 is a value unlikely to be used by any user's combination of settings and hardware
        wheelHandler.verticalStepSize = 101
        wheelHandler.horizontalStepSize = 101
        wheelScrolling()
        keyboardScrolling()
        // reset to default
        wheelHandler.verticalStepSize = undefined
        wheelHandler.horizontalStepSize = undefined
        verify(wheelHandler.verticalStepSize == 20 * Qt.styleHints.wheelScrollLines, "default verticalStepSize")
        verify(wheelHandler.horizontalStepSize == 20 * Qt.styleHints.wheelScrollLines, "default horizontalStepSize")
    }

    ScrollableFlickable {
        id: flickable
        focus: true
        anchors.fill: parent
        Kirigami.WheelHandler {
            id: wheelHandler
            target: flickable
            keyNavigationEnabled: true
        }
    }
}
