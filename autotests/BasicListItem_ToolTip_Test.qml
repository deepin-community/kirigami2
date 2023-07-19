/*
 *  SPDX-FileCopyrightText: 2023 ivan tkachenko <me@ratijas.tk>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.20 as Kirigami

// Implemented as a separate file, so it can be viewed and tweaked outside of testing framework.
Rectangle {
    id: root

    property alias itemBothNotElided:  itemBothNotElided
    property alias itemLabelElided:    itemLabelElided
    property alias itemSubtitleElided: itemSubtitleElided
    property alias itemBothElided:     itemBothElided
    property alias itemHtmlElided:     itemHtmlElided

    implicitWidth: referenceItem.implicitWidth
    implicitHeight: column.implicitHeight

    width: implicitWidth
    height: implicitHeight

    color: Kirigami.Theme.backgroundColor

    Kirigami.BasicListItem {
        id: referenceItem
        visible: false
        label: "Lorem ipsum dolor sit amet"
        subtitle: "tempor incididunt ut labore"
    }

    ColumnLayout {
        id: column
        width: parent.width
        spacing: 0

        Kirigami.BasicListItem {
            id: itemBothNotElided
            Layout.fillWidth: true
            label: "Lorem ipsum dolor"
            subtitle: "tempor incididunt"
        }
        Kirigami.BasicListItem {
            id: itemLabelElided
            Layout.fillWidth: true
            label: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod"
            subtitle: "tempor incididunt"
        }
        Kirigami.BasicListItem {
            id: itemSubtitleElided
            Layout.fillWidth: true
            label: "Lorem ipsum dolor"
            subtitle: "tempor incididunt ut labore et dolore magna aliqua."
        }
        Kirigami.BasicListItem {
            id: itemBothElided
            Layout.fillWidth: true
            label: "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod"
            subtitle: "tempor incididunt ut labore et dolore magna aliqua."
        }
        Kirigami.BasicListItem {
            id: itemHtmlElided
            Layout.fillWidth: true
            label: "HTML is <i>supported</i> inside <b>tooltips</b> as well…"
            subtitle: "…as line breaks between <sup>label</sup> &amp; <sub>subtitle</sub>"
        }
        QQC2.Label {
            Layout.fillWidth: true
            padding: Kirigami.Units.smallSpacing
            text: `QQC2.ToolTip.toolTip.visible:\n${QQC2.ToolTip.toolTip.visible}`
            color: Kirigami.Theme.textColor
        }
        QQC2.Label {
            Layout.fillWidth: true
            Layout.preferredHeight: Kirigami.Units.gridUnit * 10
            padding: Kirigami.Units.smallSpacing
            text: `QQC2.ToolTip.toolTip.text:\n"${QQC2.ToolTip.toolTip.text}"`
            wrapMode: Text.Wrap
            color: Kirigami.Theme.textColor
        }
    }
}
