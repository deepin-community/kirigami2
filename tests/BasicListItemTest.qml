/*
 *  SPDX-FileCopyrightText: 2021 Nate Graham <nate@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15 as QQC2

import org.kde.kirigami 2.20 as Kirigami

Kirigami.ApplicationWindow {
    GridLayout {
        anchors.fill: parent
        anchors.margins: Kirigami.Units.gridUnit

        rows: 3
        rowSpacing: Kirigami.Units.gridUnit
        columns: 3
        columnSpacing: Kirigami.Units.gridUnit

        // Icon + Label
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Heading {
                text: "Icon + Label"
                level: 3
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            QQC2.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component.onCompleted: {
                    background.visible = true;
                }
                ListView {
                    model: 3
                    delegate: Kirigami.BasicListItem {
                        icon: "edit-bomb"
                        text: "Boom!"
                    }
                }
            }
        }

        // Label + space reserved for icon
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Heading {
                text: "Icon + Label + space reserved for icon"
                level: 3
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            QQC2.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component.onCompleted: {
                    background.visible = true;
                }
                ListView {
                    model: 3
                    delegate: Kirigami.BasicListItem {
                        text: "Boom!"
                        reserveSpaceForIcon: true
                    }
                }
            }
        }

        // Icon + Label + leading and trailing items
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Heading {
                text: "Icon + Label + leading and trailing items"
                level: 3
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            QQC2.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component.onCompleted: {
                    background.visible = true;
                }
                ListView {
                    model: 3
                    delegate: Kirigami.BasicListItem {
                        leading: Rectangle {
                            radius: width * 0.5
                            width: Kirigami.Units.largeSpacing
                            height: Kirigami.Units.largeSpacing
                            Kirigami.Theme.colorSet: Kirigami.Theme.View
                            color: Kirigami.Theme.neutralTextColor
                        }
                        leadingFillVertically: false

                        icon: "edit-bomb"
                        text: "Boom!"

                        trailing: QQC2.Button {
                            text: "Defuse the bomb!"
                            icon.name: "edit-delete"
                        }
                    }
                }
            }
        }

        // Icon + Label + subtitle
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Heading {
                text: "Icon + Label + subtitle"
                level: 3
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            QQC2.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component.onCompleted: {
                    background.visible = true;
                }
                ListView {
                    model: 3
                    delegate: Kirigami.BasicListItem {
                        icon: "edit-bomb"
                        text: "Boom!"
                        subtitle: "smaller boom"
                    }
                }
            }
        }

        // Icon + Label + space reserved for subtitle
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Heading {
                text: "Icon + Label + space reserved for subtitle"
                level: 3
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            QQC2.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component.onCompleted: {
                    background.visible = true;
                }
                ListView {
                    model: 3
                    delegate: Kirigami.BasicListItem {
                        icon: "edit-bomb"
                        text: "Boom!"
                        reserveSpaceForSubtitle: true
                    }
                }
            }
        }

        // Icon + Label + subtitle + leading and trailing items
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Kirigami.Heading {
                text: "Icon + Label + subtitle + leading and trailing items"
                level: 3
                Layout.fillWidth: true
                wrapMode: Text.Wrap
            }
            QQC2.ScrollView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                Component.onCompleted: {
                    background.visible = true;
                }
                ListView {
                    model: 3
                    delegate: Kirigami.BasicListItem {
                        leading: Rectangle {
                            radius: width * 0.5
                            width: Kirigami.Units.largeSpacing
                            height: Kirigami.Units.largeSpacing
                            Kirigami.Theme.colorSet: Kirigami.Theme.View
                            color: Kirigami.Theme.neutralTextColor
                        }
                        leadingFillVertically: false

                        icon: "edit-bomb"
                        text: "Boom!"
                        subtitle: "smaller boom"

                        trailing: QQC2.Button {
                            text: "Defuse the bomb!"
                            icon.name: "edit-delete"
                        }
                    }
                }
            }
        }
    }
}
