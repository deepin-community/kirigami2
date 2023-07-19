/*
 *  SPDX-FileCopyrightText: 2023 ivan tkachenko <me@ratijas.tk>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.20 as Kirigami
import QtTest 1.15

// Note about tooltips: they set `visible: true` as soon as they start enter
// animation, and stay that way until exit animation completes. Actual
// tooltip instance's visible state is not the same as QQC2.ToolTip.visible
// attached property value.
// Delays (timeouts) in tests are slightly increased to mitigate possible races.
TestCase {
    name: "BasicListItemToolTip"
    visible: true
    when: windowShown

    width: ui.implicitWidth
    height: ui.implicitHeight

    BasicListItem_ToolTip_Test {
        id: ui
        anchors.fill: parent
    }

    function test_bothNotElided() {
        mouseMove(ui.itemBothNotElided, ui.itemBothNotElided.width / 2, ui.itemBothNotElided.height / 2);
        wait(QQC2.ToolTip.toolTip.delay * 1.1);
        compare(QQC2.ToolTip.toolTip.visible, false);
    }

    function test_labelElided() {
        mouseMove(ui.itemLabelElided, ui.itemLabelElided.width / 2, ui.itemLabelElided.height / 2);
        tryCompare(QQC2.ToolTip.toolTip, "visible", true, QQC2.ToolTip.toolTip.delay * 1.1);
        compare(QQC2.ToolTip.toolTip.text, ui.itemLabelElided.label);
    }

    function test_subtitleElided() {
        mouseMove(ui.itemSubtitleElided, ui.itemSubtitleElided.width / 2, ui.itemSubtitleElided.height / 2);
        tryCompare(QQC2.ToolTip.toolTip, "visible", true, QQC2.ToolTip.toolTip.delay * 1.1);
        compare(QQC2.ToolTip.toolTip.text, ui.itemSubtitleElided.subtitle);
    }

    function test_bothElided() {
        mouseMove(ui.itemBothElided, ui.itemBothElided.width / 2, ui.itemBothElided.height / 2);
        tryCompare(QQC2.ToolTip.toolTip, "visible", true, QQC2.ToolTip.toolTip.delay * 1.1);
        compare(QQC2.ToolTip.toolTip.text, `${ui.itemBothElided.label}<br/><br/>${ui.itemBothElided.subtitle}`);
    }

    function test_htmlElided() {
        mouseMove(ui.itemHtmlElided, ui.itemHtmlElided.width / 2, ui.itemHtmlElided.height / 2);
        tryCompare(QQC2.ToolTip.toolTip, "visible", true, QQC2.ToolTip.toolTip.delay * 1.1);
        compare(QQC2.ToolTip.toolTip.text, `${ui.itemHtmlElided.label}<br/><br/>${ui.itemHtmlElided.subtitle}`);
    }
}
