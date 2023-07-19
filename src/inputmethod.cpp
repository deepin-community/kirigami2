/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "inputmethod.h"

#include "libkirigami/virtualkeyboardwatcher.h"

class Q_DECL_HIDDEN InputMethod::Private
{
public:
    bool available = false;
    bool enabled = false;
    bool active = false;
    bool visible = false;
};

InputMethod::InputMethod(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<Private>())
{
    auto watcher = Kirigami::VirtualKeyboardWatcher::self();

    connect(watcher, &Kirigami::VirtualKeyboardWatcher::availableChanged, this, [this]() {
        d->available = Kirigami::VirtualKeyboardWatcher::self()->available();
        Q_EMIT availableChanged();
    });

    connect(watcher, &Kirigami::VirtualKeyboardWatcher::enabledChanged, this, [this]() {
        d->enabled = Kirigami::VirtualKeyboardWatcher::self()->enabled();
        Q_EMIT enabledChanged();
    });

    connect(watcher, &Kirigami::VirtualKeyboardWatcher::activeChanged, this, [this]() {
        d->active = Kirigami::VirtualKeyboardWatcher::self()->active();
        Q_EMIT activeChanged();
    });

    connect(watcher, &Kirigami::VirtualKeyboardWatcher::visibleChanged, this, [this]() {
        d->visible = Kirigami::VirtualKeyboardWatcher::self()->visible();
        Q_EMIT visibleChanged();
    });

    connect(watcher, &Kirigami::VirtualKeyboardWatcher::willShowOnActiveChanged, this, [this]() {
        Q_EMIT willShowOnActiveChanged();
    });

    d->available = watcher->available();
    d->enabled = watcher->enabled();
    d->active = watcher->active();
    d->visible = watcher->visible();
}

InputMethod::~InputMethod() = default;

bool InputMethod::available() const
{
    return d->available;
}

bool InputMethod::enabled() const
{
    return d->enabled;
}

void InputMethod::setEnabled(bool newEnabled)
{
    if (newEnabled == d->enabled) {
        return;
    }

    d->enabled = newEnabled;
    Q_EMIT enabledChanged();
}

bool InputMethod::active() const
{
    return d->active;
}

void InputMethod::setActive(bool newActive)
{
    if (newActive == d->active) {
        return;
    }

    d->active = newActive;
    Q_EMIT activeChanged();
}

bool InputMethod::visible() const
{
    return d->visible;
}

bool InputMethod::willShowOnActive() const
{
    return Kirigami::VirtualKeyboardWatcher::self()->willShowOnActive();
}
