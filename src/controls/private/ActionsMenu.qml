/*
 *  SPDX-FileCopyrightText: 2018 Aleix Pol Gonzalez <aleixpol@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import org.kde.kirigami 2.20 as Kirigami

QQC2.Menu
{
    id: theMenu
    z: 999999999
    property alias actions: actionsInstantiator.model
    property Component submenuComponent
    // renamed to work on both Qt 5.9 and 5.10
    property Component itemDelegate: ActionMenuItem {}
    property Component separatorDelegate: QQC2.MenuSeparator { property var action }
    property Component loaderDelegate: Loader { property var action }
    property QQC2.Action parentAction
    property QQC2.MenuItem parentItem

    Item {
        id: invisibleItems
        visible: false
    }
    Instantiator {
        id: actionsInstantiator

        active: theMenu.visible
        delegate: QtObject {
            readonly property QQC2.Action action: modelData
            property QtObject item: null
            property bool isSubMenu: false

            function create() {
                if (!action.hasOwnProperty("children") && !action.children || action.children.length === 0) {
                    if (action.hasOwnProperty("separator") && action.separator) {
                        item = theMenu.separatorDelegate.createObject(null, { action: action });
                    }
                    else if (action.displayComponent) {
                        item = theMenu.loaderDelegate.createObject(null,
                                { action: action, sourceComponent: action.displayComponent });
                    }
                    else {
                        item = theMenu.itemDelegate.createObject(null, { action: action });
                    }
                    theMenu.addItem(item)
                } else if (theMenu.submenuComponent) {
                    item = theMenu.submenuComponent.createObject(null, { parentAction: action, title: action.text, actions: action.children });

                    theMenu.insertMenu(theMenu.count, item)
                    item.parentItem = theMenu.contentData[theMenu.contentData.length-1]
                    item.parentItem.icon = action.icon
                    isSubMenu = true
                }
            }
            function remove() {
                if (isSubMenu) {
                    theMenu.removeMenu(item)
                } else {
                    theMenu.removeItem(item)
                }
                item.destroy()
            }
        }

        onObjectAdded: object.create()
        onObjectRemoved: object.remove()
    }
}
