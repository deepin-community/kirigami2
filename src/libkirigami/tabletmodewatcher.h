/*
 *  SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KIRIGAMI_TABLETMODEWATCHER
#define KIRIGAMI_TABLETMODEWATCHER

#include <QEvent>
#include <QObject>

#include "kirigami2_export.h"

namespace Kirigami
{
class TabletModeWatcherPrivate;

class KIRIGAMI2_EXPORT TabletModeChangedEvent : public QEvent
{
public:
    TabletModeChangedEvent(bool tablet)
        : QEvent(TabletModeChangedEvent::type)
        , tabletMode(tablet)
    {
    }

    bool tabletMode = false;

    static QEvent::Type type;
};

/**
 * @class TabletModeWatcher tabletmodewatcher.h <Kirigami/TabletModeWatcher>
 *
 * This class reports on the status of certain transformable
 * devices which can be both tablets and laptops at the same time,
 * with a detachable keyboard.
 * It reports whether the device supports a tablet mode and if
 * the device is currently in such mode or not, emitting a signal
 * when the user switches.
 */
#ifdef KIRIGAMI_BUILD_TYPE_STATIC
class TabletModeWatcher : public QObject
#else
class KIRIGAMI2_EXPORT TabletModeWatcher : public QObject
#endif
{
    Q_OBJECT
    Q_PROPERTY(bool tabletModeAvailable READ isTabletModeAvailable NOTIFY tabletModeAvailableChanged)
    Q_PROPERTY(bool tabletMode READ isTabletMode NOTIFY tabletModeChanged)

public:
    ~TabletModeWatcher() override;
    static TabletModeWatcher *self();

    /**
     * @returns true if the device supports a tablet mode and has a switch
     * to report when the device has been transformed.
     * For debug purposes, if either the environment variable QT_QUICK_CONTROLS_MOBILE
     * or KDE_KIRIGAMI_TABLET_MODE are set to true, isTabletModeAvailable will be true
     */
    bool isTabletModeAvailable() const;

    /**
     * @returns true if the machine is now in tablet mode, such as the
     * laptop keyboard flipped away or detached.
     * Note that this doesn't mean exactly a tablet form factor, but
     * that the preferred input mode for the device is the touch screen
     * and that pointer and keyboard are either secondary or not available.
     *
     * For debug purposes, if either the environment variable QT_QUICK_CONTROLS_MOBILE
     * or KDE_KIRIGAMI_TABLET_MODE are set to true, isTabletMode will be true
     */
    bool isTabletMode() const;

    /**
     * Register an arbitrary QObject to send events from this.
     * At the moment only one event will be sent: TabletModeChangedEvent
     */
    void addWatcher(QObject *watcher);

    /*
     * Unsubscribe watcher from receiving events from TabletModeWatcher.
     */
    void removeWatcher(QObject *watcher);

Q_SIGNALS:
    void tabletModeAvailableChanged(bool tabletModeAvailable);
    void tabletModeChanged(bool tabletMode);

private:
    TabletModeWatcher(QObject *parent = nullptr);
    TabletModeWatcherPrivate *d;
    friend class TabletModeWatcherSingleton;
};
}

#endif // KIRIGAMI_TABLETMODEWATCHER
