/*
 *  SPDX-FileCopyrightText: 2017 by Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef KIRIGAMIPLUGINFACTORY_H
#define KIRIGAMIPLUGINFACTORY_H

#include "platformtheme.h"
#include <QObject>

#ifndef KIRIGAMI_BUILD_TYPE_STATIC
#include <kirigami2_export.h>
#endif

class QQmlEngine;

namespace Kirigami {
class Units;

/**
 * @class KirigamiPluginFactory kirigamipluginfactory.h KirigamiPluginFactory
 *
 * This class is reimpleented by plugins to provide different implementations
 * of PlatformTheme
 */
#ifdef KIRIGAMI_BUILD_TYPE_STATIC
class KirigamiPluginFactory : public QObject
#else
class KIRIGAMI2_EXPORT KirigamiPluginFactory : public QObject
#endif
{
    Q_OBJECT

public:
    explicit KirigamiPluginFactory(QObject *parent = nullptr);
    ~KirigamiPluginFactory() override;

    /**
     * Creates an instance of PlatformTheme which can come out from
     * an implementation provided by a plugin
     *
     * If this returns nullptr the PlatformTheme will use a fallback
     * implementation that loads a theme definition from a QML file.
     *
     * @param parent the parent object of the created PlatformTheme
     */
    virtual PlatformTheme *createPlatformTheme(QObject *parent) = 0;

    /**
     * finds the plugin providing units and platformtheme for the current style
     * The plugin pointer is cached, so only the first call is a potentially heavy operation
     * @return pointer to the KirigamiPluginFactory of the current style
     */
    static KirigamiPluginFactory *findPlugin();
};

// TODO KF6 unify KirigamiPluginFactory and KirigamiPluginFactoryV2 again
/**
 * This class provides an extended version of KirigamiPluginFactory.
 * Plugins that support Units need to implement it instead of KirigamiPluginFactory.
 */
#ifdef KIRIGAMI_BUILD_TYPE_STATIC
class KirigamiPluginFactoryV2 : public KirigamiPluginFactory
#else
class KIRIGAMI2_EXPORT KirigamiPluginFactoryV2 : public KirigamiPluginFactory
#endif
{
    Q_OBJECT

public:
    explicit KirigamiPluginFactoryV2(QObject *parent = nullptr);
    ~KirigamiPluginFactoryV2() override;

    /**
     * Creates an instance of Units which can come from an implementation
     * provided by a plugin
     * @param parent the parent of the units object
     */
    virtual Units *createUnits(QObject *parent) = 0;
};
}

QT_BEGIN_NAMESPACE
#define KirigamiPluginFactory_iid "org.kde.kirigami.KirigamiPluginFactory"
Q_DECLARE_INTERFACE(Kirigami::KirigamiPluginFactory, KirigamiPluginFactory_iid)
QT_END_NAMESPACE

#endif // KIRIGAMIPLUGINFACTORY_H
