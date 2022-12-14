/*
 *  SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.6
import org.kde.kirigami 2.12
import "templates" as T
import "private"

/**
 * A AbstractCard is the base for cards. A Card is a visual object that serves
 * as an entry point for more detailed information. An abstractCard is empty,
 * providing just the look and the base properties and signals for an ItemDelegate.
 * It can be filled with any custom layout of items, its content is organized
 * in 3 properties: header, contentItem and footer.
 * Use this only when you need particular custom contents, for a standard layout
 * for cards, use the Card component.
 *
 * @see Card
 * @inherit org::kde::kirigami::templates::AbstractCard
 * @since 2.4
 */
T.AbstractCard {
    id: root

    background: DefaultCardBackground {
        id: bg

        clickFeedback: root.showClickFeedback
        hoverFeedback: root.hoverEnabled
    }
}
