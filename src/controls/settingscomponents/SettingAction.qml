/*
 *  SPDX-FileCopyrightText: 2021 Felipe Kinoshita <kinofhek@gmail.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.15
import QtQuick.Controls 2.15 as QQC2
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.11 as Kirigami

/**
 * Each SettingAction given to a CategorizedSettings is used to
 * declare a specific setting page.
 *
 * @since 5.86
 * @since org.kde.kirigami 2.18
 *
 * @inherits org::kde::kirigami::PagePoolAction
 */
Kirigami.PagePoolAction {
    pageStack: stack
    pagePool: pool
    basePage: stack.initialPage

    checkable: false
}
