/*
 *  SPDX-FileCopyrightText: 2016 by Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import org.kde.kirigami 2.12 as Kirigami
import "private" as P
import "templates" as T

/**
 * @brief An overlay sheet that covers the current Page content.
 *
 * Its contents can be scrolled up or down, scrolling all the way up or
 * all the way down, dismisses it.
 * Use this for big, modal dialogs or information display, that can't be
 * logically done as a new separate Page, even if potentially
 * are taller than the screen space.
 * @inherit org::kde::kirigami::templates::OverlaySheet
 */
T.OverlaySheet {
    id: root

    leftInset: 0
    topInset: -Kirigami.Units.smallSpacing
    rightInset: 0
    bottomInset: -Kirigami.Units.smallSpacing

    background: P.DefaultCardBackground {}
}
