/*
 *  SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>
 *  SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "virtualkeyboardwatcher.h"

#ifdef KIRIGAMI_ENABLE_DBUS
#include "virtualkeyboard_interface.h"
#include <QDBusConnection>
#include <QDBusPendingCallWatcher>
#endif

#include "loggingcategory.h"

namespace Kirigami
{
Q_GLOBAL_STATIC(VirtualKeyboardWatcher, virtualKeyboardWatcherSelf)

class Q_DECL_HIDDEN VirtualKeyboardWatcher::Private
{
public:
    Private(VirtualKeyboardWatcher *qq)
        : q(qq)
    {
    }

    VirtualKeyboardWatcher *q;

#ifdef KIRIGAMI_ENABLE_DBUS
    void getAllProperties();
    void getProperty(const QString &propertyName);
    void updateWillShowOnActive();

    OrgKdeKwinVirtualKeyboardInterface *keyboardInterface = nullptr;
    OrgFreedesktopDBusPropertiesInterface *propertiesInterface = nullptr;

    QDBusPendingCallWatcher *willShowOnActiveCall = nullptr;
#endif

    bool available = false;
    bool enabled = false;
    bool active = false;
    bool visible = false;
    bool willShowOnActive = false;

    static const QString serviceName;
    static const QString objectName;
    static const QString interfaceName;
};

const QString VirtualKeyboardWatcher::Private::serviceName = QStringLiteral("org.kde.KWin");
const QString VirtualKeyboardWatcher::Private::objectName = QStringLiteral("/VirtualKeyboard");
const QString VirtualKeyboardWatcher::Private::interfaceName = QStringLiteral("org.kde.kwin.VirtualKeyboard");

VirtualKeyboardWatcher::VirtualKeyboardWatcher(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<Private>(this))
{
#ifdef KIRIGAMI_ENABLE_DBUS
    d->keyboardInterface = new OrgKdeKwinVirtualKeyboardInterface(Private::serviceName, Private::objectName, QDBusConnection::sessionBus(), this);
    d->propertiesInterface = new OrgFreedesktopDBusPropertiesInterface(Private::serviceName, Private::objectName, QDBusConnection::sessionBus(), this);

    connect(d->keyboardInterface, &OrgKdeKwinVirtualKeyboardInterface::availableChanged, this, [this]() {
        d->getProperty(QStringLiteral("available"));
    });
    connect(d->keyboardInterface, &OrgKdeKwinVirtualKeyboardInterface::enabledChanged, this, [this]() {
        d->getProperty(QStringLiteral("enabled"));
    });
    connect(d->keyboardInterface, &OrgKdeKwinVirtualKeyboardInterface::activeChanged, this, [this]() {
        d->getProperty(QStringLiteral("active"));
    });
    connect(d->keyboardInterface, &OrgKdeKwinVirtualKeyboardInterface::visibleChanged, this, [this]() {
        d->getProperty(QStringLiteral("visible"));
    });

    d->getAllProperties();
#endif
}

VirtualKeyboardWatcher::~VirtualKeyboardWatcher() = default;

bool VirtualKeyboardWatcher::available() const
{
    return d->available;
}

bool VirtualKeyboardWatcher::enabled() const
{
    return d->enabled;
}

void VirtualKeyboardWatcher::setEnabled(bool newEnabled)
{
    if (newEnabled == d->enabled) {
        return;
    }

    d->enabled = newEnabled;

#ifdef KIRIGAMI_ENABLE_DBUS
    d->propertiesInterface->Set(Private::interfaceName, QStringLiteral("enabled"), QDBusVariant(newEnabled));
#else
    Q_EMIT enabledChanged();
#endif
}

bool VirtualKeyboardWatcher::active() const
{
    return d->active;
}

void VirtualKeyboardWatcher::setActive(bool newActive)
{
    if (newActive == d->active) {
        return;
    }

    d->active = newActive;

#ifdef KIRIGAMI_ENABLE_DBUS
    d->propertiesInterface->Set(Private::interfaceName, QStringLiteral("active"), QDBusVariant(newActive));
#else
    Q_EMIT activeChanged();
#endif
}

bool VirtualKeyboardWatcher::visible() const
{
    return d->visible;
}

bool VirtualKeyboardWatcher::willShowOnActive() const
{
#ifdef KIRIGAMI_ENABLE_DBUS
    d->updateWillShowOnActive();
#endif
    return d->willShowOnActive;
}

VirtualKeyboardWatcher *VirtualKeyboardWatcher::self()
{
    return virtualKeyboardWatcherSelf();
}

#ifdef KIRIGAMI_ENABLE_DBUS

void VirtualKeyboardWatcher::Private::updateWillShowOnActive()
{
    if (willShowOnActiveCall) {
        return;
    }

    willShowOnActiveCall = new QDBusPendingCallWatcher(keyboardInterface->willShowOnActive(), q);
    connect(willShowOnActiveCall, &QDBusPendingCallWatcher::finished, q, [this](auto call) {
        QDBusPendingReply<bool> reply = *call;
        if (reply.isError()) {
            qCDebug(KirigamiLog) << reply.error().message();
        } else {
            if (reply.value() != willShowOnActive) {
                willShowOnActive = reply.value();
                Q_EMIT q->willShowOnActiveChanged();
            }
        }
        call->deleteLater();
        willShowOnActiveCall = nullptr;
    });
}

void VirtualKeyboardWatcher::Private::getAllProperties()
{
    auto call = new QDBusPendingCallWatcher(propertiesInterface->GetAll(interfaceName), q);
    connect(call, &QDBusPendingCallWatcher::finished, q, [this](auto call) {
        QDBusPendingReply<QVariantMap> reply = *call;
        if (reply.isError()) {
            qCDebug(KirigamiLog) << reply.error().message();
        } else {
            auto value = reply.value();
            available = value.value(QStringLiteral("available")).toBool();
            enabled = value.value(QStringLiteral("enabled")).toBool();
            active = value.value(QStringLiteral("active")).toBool();
            visible = value.value(QStringLiteral("visible")).toBool();
        }
        call->deleteLater();

        Q_EMIT q->availableChanged();
        Q_EMIT q->enabledChanged();
        Q_EMIT q->activeChanged();
        Q_EMIT q->visibleChanged();
    });
}

void VirtualKeyboardWatcher::Private::getProperty(const QString &propertyName)
{
    auto call = new QDBusPendingCallWatcher(propertiesInterface->Get(interfaceName, propertyName), q);
    connect(call, &QDBusPendingCallWatcher::finished, q, [this, propertyName](auto call) {
        QDBusPendingReply<QDBusVariant> reply = *call;
        if (reply.isError()) {
            qCDebug(KirigamiLog) << reply.error().message();
        } else {
            auto value = reply.value();
            if (propertyName == QStringLiteral("available")) {
                available = value.variant().toBool();
                Q_EMIT q->availableChanged();
            } else if (propertyName == QStringLiteral("enabled")) {
                enabled = value.variant().toBool();
                Q_EMIT q->enabledChanged();
            } else if (propertyName == QStringLiteral("active")) {
                active = value.variant().toBool();
                Q_EMIT q->activeChanged();
            } else if (propertyName == QStringLiteral("visible")) {
                visible = value.variant().toBool();
                Q_EMIT q->visibleChanged();
            }
        }
        call->deleteLater();
    });
}

#endif
}
