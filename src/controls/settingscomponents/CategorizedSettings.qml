/*
 *  SPDX-FileCopyrightText: 2020 Tobias Fella <fella@posteo.de>
 *  SPDX-FileCopyrightText: 2021 Carl Schwan <carl@carlschwan.eu>
 *  SPDX-FileCopyrightText: 2021 Felipe Kinoshita <kinofhek@gmail.com>
 *  SPDX-FileCopyrightText: 2021 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.11

/**
 * A container for setting actions showing them in a list view and displaying
 * the actual page next to it.
 *
 * @since 5.86
 * @since org.kde.kirigami 2.18
 */
PageRow {
    id: pageSettingStack

    property list<PagePoolAction> actions
    property alias stack: pageSettingStack
    property PagePool pool: PagePool {}

    readonly property string title: pageSettingStack.depth < 2 ? qsTr("Settings") : qsTr("Settings — %1").arg(pageSettingStack.get(1).title)

    bottomPadding: 0
    leftPadding: 0
    rightPadding: 0
    topPadding: 0

    columnView.columnWidth: Units.gridUnit * 7 // So it's the same size as the kxmlgui settings dialogs
    globalToolBar.showNavigationButtons: ApplicationHeaderStyle.NoNavigationButtons
    globalToolBar.style: ApplicationHeaderStyle.Breadcrumb

    signal backRequested(var event)
    onBackRequested: {
        if (pageSettingStack.depth > 1 && !pageSettingStack.wideMode && pageSettingStack.currentIndex !== 0) {
            event.accepted = true;
            pageSettingStack.pop();
        }
    }
    onWidthChanged: if (pageSettingStack.depth < 2 && pageSettingStack.width >= Units.gridUnit * 40) {
        actions[0].trigger();
    }

    initialPage: ScrollablePage {
        title: qsTr("Settings")
        bottomPadding: 0
        leftPadding: 0
        rightPadding: 0
        topPadding: 0
        Theme.colorSet: Theme.View
        ListView {
            id: listview
            Component.onCompleted: if (pageSettingStack.width >= Units.gridUnit * 40) {
                actions[0].trigger();
            } else {
                listview.currentIndex = -1;
            }
            model: pageSettingStack.actions
            delegate: pageSettingStack.wideMode ? desktopStyle : mobileStyle
        }
    }

    Component {
        id: desktopStyle

        QQC2.ItemDelegate {
            width: parent && parent.width > 0 ? parent.width : implicitWidth
            implicitWidth: contentItem.implicitWidth + Units.smallSpacing * 4
            implicitHeight: contentItem.implicitHeight + Units.smallSpacing * 2
            highlighted: ListView.isCurrentItem

            action: modelData
            onClicked: listview.currentIndex = index
            contentItem: ColumnLayout {
                spacing: Units.smallSpacing

                Icon {
                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredWidth: Units.iconSizes.medium
                    Layout.preferredHeight: width
                    source: modelData.icon.name
                }

                QQC2.Label {
                    Layout.fillWidth: true
                    Layout.leftMargin: Units.smallSpacing
                    Layout.rightMargin: Units.smallSpacing
                    text: modelData.text
                    wrapMode: Text.Wrap
                    color: highlighted ? Theme.highlightedTextColor : Theme.textColor
                    horizontalAlignment: Text.AlignHCenter
                }
            }

        }
    }

    Component {
        id: mobileStyle

        BasicListItem {
            action: modelData
            onClicked: {
                listview.currentIndex = index;
            }
        }
    }
}

