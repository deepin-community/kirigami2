/*
 *  SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtTest 1.0
import org.kde.kirigami 2.11 as Kirigami

TestCase {
    id: testCase
    name: "ThemeTest"

    width: 400
    height: 400
    visible: true

    when: windowShown

    TextMetrics {
        id: textMetrics
    }

    // Not all properties are updated immediately to avoid having massive storms
    // of duplicated signals and to prevent changes from retriggering code that
    // changed it. To deal with that, we need to wait a bit before continuiing
    // when we change properties. This time shouldn't be too short because on
    // some machines it may take a bit longer for things to properly be updated.
    function waitForEvents()
    {
        wait(20)
    }

    Component {
        id: basic

        Rectangle {
            color: Kirigami.Theme.backgroundColor

            property alias text: textItem

            Text {
                id: textItem
                color: Kirigami.Theme.textColor
                font: Kirigami.Theme.defaultFont
            }
        }
    }

    function test_basic() {
        var item = createTemporaryObject(basic, testCase)
        verify(item)

        compare(item.Kirigami.Theme.colorSet, Kirigami.Theme.Window)
        compare(item.Kirigami.Theme.colorGroup, Kirigami.Theme.Active)
        verify(item.Kirigami.Theme.inherit)

        compare(item.color, "#eff0f1")
        compare(item.text.color, "#31363b")
        compare(item.text.font.family, textMetrics.font.family)
    }

    Component {
        id: override

        Rectangle {
            Kirigami.Theme.backgroundColor: "#ff0000"
            color: Kirigami.Theme.backgroundColor
        }
    }

    function test_override() {
        var item = createTemporaryObject(override, testCase)
        verify(item)

        compare(item.color, "#ff0000")

        item.Kirigami.Theme.backgroundColor = "#00ff00"

        // Changes to Theme are not immediately propagated, so give it a few
        // moments.
        waitForEvents()

        compare(item.color, "#00ff00")

        // Changing colorSet or colorGroup does not affect local overrides
        item.Kirigami.Theme.colorSet = Kirigami.Theme.Complementary
        item.Kirigami.Theme.colorGroup = Kirigami.Theme.Disabled

        waitForEvents()

        compare(item.color, "#00ff00")
    }

    Component {
        id: inherit

        Rectangle {
            color: Kirigami.Theme.backgroundColor

            property alias child1: rect1
            property alias child2: rect2

            Rectangle {
                id: rect1
                color: Kirigami.Theme.backgroundColor
            }
            Rectangle {
                id: rect2
                Kirigami.Theme.inherit: false
                color: Kirigami.Theme.backgroundColor
            }
        }
    }

    function test_inherit() {
        var item = createTemporaryObject(inherit, testCase)
        verify(item)

        // Default values are all the same
        compare(item.color, "#eff0f1")
        compare(item.child1.color, "#eff0f1")
        compare(item.child2.color, "#eff0f1")

        // If we change the colorSet, the item that inherits gets updated, but
        // the item that does not stays the same.
        item.Kirigami.Theme.colorSet = Kirigami.Theme.View

        waitForEvents()

        compare(item.color, "#fcfcfc")
        compare(item.child1.color, "#fcfcfc")
        compare(item.child2.color, "#eff0f1")

        // If we override a color, the item that inherits gets that color, while
        // the item that does not ignores it.
        item.Kirigami.Theme.backgroundColor = "#ff0000"

        waitForEvents()

        compare(item.color, "#ff0000")
        compare(item.child1.color, "#ff0000")
        compare(item.child2.color, "#eff0f1")

        // If we change the color set again, the overridden color remains the
        // same for both the original object and the inherited object.
        item.Kirigami.Theme.colorSet = Kirigami.Theme.View

        waitForEvents()

        compare(item.color, "#ff0000")
        compare(item.child1.color, "#ff0000")
        compare(item.child2.color, "#eff0f1")

        // If we override a color of the item that inherits, it will stay the
        // same even if that color changes in the parent.
        item.child1.Kirigami.Theme.backgroundColor = "#00ff00"
        item.Kirigami.Theme.backgroundColor = "#0000ff"

        waitForEvents()

        compare(item.color, "#0000ff")
        compare(item.child1.color, "#00ff00")
        compare(item.child2.color, "#eff0f1")
    }

    Component {
        id: deepInherit

        Rectangle {
            color: Kirigami.Theme.backgroundColor

            property alias child1: rect1
            property alias child2: rect2
            property alias child3: rect3

            Rectangle {
                id: rect1
                color: Kirigami.Theme.backgroundColor

                Rectangle {
                    id: rect2
                    color: Kirigami.Theme.backgroundColor

                    Rectangle {
                        id: rect3
                        color: Kirigami.Theme.backgroundColor
                    }
                }
            }
        }
    }

    function test_inherit_deep() {
        var item = createTemporaryObject(deepInherit, testCase)
        verify(item)

        waitForEvents()

        compare(item.color, "#eff0f1")
        compare(item.child1.color, "#eff0f1")
        compare(item.child2.color, "#eff0f1")
        compare(item.child3.color, "#eff0f1")

        item.Kirigami.Theme.backgroundColor = "#ff0000"

        waitForEvents()

        compare(item.color, "#ff0000")
        compare(item.child1.color, "#ff0000")
        compare(item.child2.color, "#ff0000")
        compare(item.child3.color, "#ff0000")

        item.child2.Kirigami.Theme.inherit = false
        item.child2.Kirigami.Theme.backgroundColor = "#00ff00"

        waitForEvents()

        compare(item.color, "#ff0000")
        compare(item.child1.color, "#ff0000")
        compare(item.child2.color, "#00ff00")
        compare(item.child3.color, "#00ff00")

        item.child2.Kirigami.Theme.inherit = true
        item.child2.Kirigami.Theme.backgroundColor = undefined

        waitForEvents()

        compare(item.color, "#ff0000")
        compare(item.child1.color, "#ff0000")
        compare(item.child2.color, "#ff0000")
        compare(item.child3.color, "#ff0000")

        item.child2.Kirigami.Theme.colorSet = Kirigami.Theme.Complementary
        item.child2.Kirigami.Theme.inherit = false

        waitForEvents()

        compare(item.color, "#ff0000")
        compare(item.child1.color, "#ff0000")
        compare(item.child2.color, "#31363b")
        compare(item.child3.color, "#31363b")
    }

    Component {
        id: colorSet

        Rectangle {
            Kirigami.Theme.colorSet: Kirigami.Theme.View
            color: Kirigami.Theme.backgroundColor
        }
    }

    function test_colorset() {
        var item = createTemporaryObject(colorSet, testCase)
        verify(item)

        waitForEvents()

        compare(item.color, "#fcfcfc")

        item.Kirigami.Theme.colorSet = Kirigami.Theme.Complementary

        waitForEvents()

        compare(item.color, "#31363b")
    }

    Component {
        id: colorGroup

        Rectangle {
            Kirigami.Theme.colorGroup: Kirigami.Theme.Disabled
            color: Kirigami.Theme.backgroundColor
        }
    }

    function test_colorGroup() {
        var item = createTemporaryObject(colorGroup, testCase)
        verify(item)

        waitForEvents()

        var color = Qt.tint("#eff0f1", "transparent")

        compare(item.color, Qt.hsva(color.hsvHue, color.hsvSaturation * 0.5, color.hsvValue * 0.8))

        item.Kirigami.Theme.colorGroup = Kirigami.Theme.Inactive

        waitForEvents()

        compare(item.color, Qt.hsva(color.hsvHue, color.hsvSaturation * 0.5, color.hsvValue))
    }

    Component {
        id: palette

        Rectangle {
            color: Kirigami.Theme.backgroundColor

            property alias child: button

            Button {
                id: button
                palette: Kirigami.Theme.palette
            }
        }
    }

    function test_palette() {
        var item = createTemporaryObject(palette, testCase)
        verify(item)

        compare(item.child.background.color, "#eff0f1")
        compare(item.child.contentItem.color, "#31363b")

        item.Kirigami.Theme.backgroundColor = "#ff0000"

        waitForEvents()

        compare(item.child.background.color, "#ff0000")
    }
}
