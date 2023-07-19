/*
 *  SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "tabletmodewatcher.h"
#include <QCoreApplication>

#if defined(KIRIGAMI_ENABLE_DBUS)
#include "tabletmodemanager_interface.h"
#include <QDBusConnection>
#endif

// TODO: All the dbus stuff should be conditional, optional win32 support

namespace Kirigami
{
KIRIGAMI2_EXPORT QEvent::Type TabletModeChangedEvent::type = QEvent::None;

class TabletModeWatcherSingleton
{
public:
    TabletModeWatcher self;
};

Q_GLOBAL_STATIC(TabletModeWatcherSingleton, privateTabletModeWatcherSelf)

class TabletModeWatcherPrivate
{
public:
    TabletModeWatcherPrivate(TabletModeWatcher *watcher)
        : q(watcher)
    {
        // Called here to avoid collisions with application event types so we should use
        // registerEventType for generating the event types.
        TabletModeChangedEvent::type = QEvent::Type(QEvent::registerEventType());
#if !defined(KIRIGAMI_ENABLE_DBUS) && (defined(Q_OS_ANDROID) || defined(Q_OS_IOS))
        isTabletModeAvailable = true;
        isTabletMode = true;
#elif defined(KIRIGAMI_ENABLE_DBUS)
        // Mostly for debug purposes and for platforms which are always mobile,
        // such as Plasma Mobile
        if (qEnvironmentVariableIsSet("QT_QUICK_CONTROLS_MOBILE") || qEnvironmentVariableIsSet("KDE_KIRIGAMI_TABLET_MODE")) {
            /* clang-format off */
            isTabletMode =
                (QString::fromLatin1(qgetenv("QT_QUICK_CONTROLS_MOBILE")) == QStringLiteral("1")
                    || QString::fromLatin1(qgetenv("QT_QUICK_CONTROLS_MOBILE")) == QStringLiteral("true"))
                || (QString::fromLatin1(qgetenv("KDE_KIRIGAMI_TABLET_MODE")) == QStringLiteral("1")
                    || QString::fromLatin1(qgetenv("KDE_KIRIGAMI_TABLET_MODE")) == QStringLiteral("true"));
            /* clang-format on */
            isTabletModeAvailable = isTabletMode;
        } else {
            m_interface =
                new OrgKdeKWinTabletModeManagerInterface(QStringLiteral("org.kde.KWin"), QStringLiteral("/org/kde/KWin"), QDBusConnection::sessionBus(), q);

            if (m_interface->isValid()) {
                // NOTE: the initial call is actually sync, because is better a tiny freeze than having the ui always recalculated and changed at the start
                QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.kde.KWin"),
                                                                      QStringLiteral("/org/kde/KWin"),
                                                                      QStringLiteral("org.freedesktop.DBus.Properties"),
                                                                      QStringLiteral("GetAll"));
                message.setArguments({m_interface->interface()});
                QDBusReply<QVariantMap> propsReply = QDBusConnection::sessionBus().call(message);
                isTabletModeAvailable = propsReply.value()[QLatin1String("tabletModeAvailable")].toBool();
                isTabletMode = propsReply.value()[QLatin1String("tabletMode")].toBool();
                QObject::connect(m_interface, &OrgKdeKWinTabletModeManagerInterface::tabletModeChanged, q, [this](bool tabletMode) {
                    setIsTablet(tabletMode);
                });
                QObject::connect(m_interface, &OrgKdeKWinTabletModeManagerInterface::tabletModeAvailableChanged, q, [this](bool avail) {
                    isTabletModeAvailable = avail;
                    Q_EMIT q->tabletModeAvailableChanged(avail);
                });
            } else {
                isTabletModeAvailable = false;
                isTabletMode = false;
            }
        }
// TODO: case for Windows
#else
        isTabletModeAvailable = false;
        isTabletMode = false;
#endif
    }
    ~TabletModeWatcherPrivate(){};
    void setIsTablet(bool tablet);

    TabletModeWatcher *q;
#if defined(KIRIGAMI_ENABLE_DBUS)
    OrgKdeKWinTabletModeManagerInterface *m_interface = nullptr;
#endif
    QVector<QObject *> watchers;
    bool isTabletModeAvailable = false;
    bool isTabletMode = false;
};

void TabletModeWatcherPrivate::setIsTablet(bool tablet)
{
    if (isTabletMode == tablet) {
        return;
    }

    isTabletMode = tablet;
    TabletModeChangedEvent event{tablet};
    Q_EMIT q->tabletModeChanged(tablet);
    for (auto *w : watchers) {
        QCoreApplication::sendEvent(w, &event);
    }
}

TabletModeWatcher::TabletModeWatcher(QObject *parent)
    : QObject(parent)
    , d(new TabletModeWatcherPrivate(this))
{
}

TabletModeWatcher::~TabletModeWatcher()
{
    delete d;
}

TabletModeWatcher *TabletModeWatcher::self()
{
    return &privateTabletModeWatcherSelf()->self;
}

bool TabletModeWatcher::isTabletModeAvailable() const
{
    return d->isTabletModeAvailable;
}

bool TabletModeWatcher::isTabletMode() const
{
    return d->isTabletMode;
}

void TabletModeWatcher::addWatcher(QObject *watcher)
{
    d->watchers.append(watcher);
}

void TabletModeWatcher::removeWatcher(QObject *watcher)
{
    d->watchers.removeAll(watcher);
}
}

#include "moc_tabletmodewatcher.cpp"
