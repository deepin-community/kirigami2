// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: GPL-2.0-or-later

import "templates" as T
import "private" as P

/**
 * AbstractChip is a visual object based on AbstractButton
 * that provides a way to display predetermined elements
 * with the visual styling of "tags" or "tokens". It provides
 * the look, the base properties, and signals for an AbstractButton.
 *
 * @inherit org::kde::kirigami::templates::AbstractChip
 * @since 2.19
 */
T.AbstractChip {
    id: root

    background: P.DefaultChipBackground {}
}
