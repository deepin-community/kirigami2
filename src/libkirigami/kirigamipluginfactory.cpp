/*
 *  SPDX-FileCopyrightText: 2017 by Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "kirigamipluginfactory.h"

#include <QDebug>

#include <QDir>
#include <QQuickStyle>
#include <QPluginLoader>

#include "styleselector_p.h"
#include "units.h"

#include "loggingcategory.h"

namespace Kirigami {

KirigamiPluginFactory::KirigamiPluginFactory(QObject *parent)
    : QObject(parent)
{
}

KirigamiPluginFactory::~KirigamiPluginFactory() = default;

KirigamiPluginFactory *KirigamiPluginFactory::findPlugin()
{
    static KirigamiPluginFactory *pluginFactory = nullptr;

    //check for the plugin only once: it's an heavy operation
    if (pluginFactory) {
        return pluginFactory;
    }

    static bool s_factoryChecked = false;

    if (!s_factoryChecked) {
        s_factoryChecked = true;

        #ifdef KIRIGAMI_BUILD_TYPE_STATIC
        for (QObject *staticPlugin : QPluginLoader::staticInstances()) {
            KirigamiPluginFactory *factory = qobject_cast<KirigamiPluginFactory *>(staticPlugin);
            if (factory) {
                pluginFactory = factory;
            }
        }
        #else
        const auto libraryPaths = QCoreApplication::libraryPaths();
        for (const QString &path : libraryPaths) {
            #ifdef Q_OS_ANDROID
            QDir dir(path);
            #else
            QDir dir(path + QStringLiteral("/kf" QT_STRINGIFY(QT_VERSION_MAJOR) "/kirigami"));
            #endif
            const auto fileNames = dir.entryList(QDir::Files);

            for (const QString &fileName : fileNames) {

#ifdef Q_OS_ANDROID
                if (fileName.startsWith(QStringLiteral("libplugins_kf" QT_STRINGIFY(QT_VERSION_MAJOR) "_kirigami_")) && QLibrary::isLibrary(fileName)) {
#endif
                    // TODO: env variable?
                    if (!QQuickStyle::name().isEmpty() && fileName.contains(QQuickStyle::name())) {
                        QPluginLoader loader(dir.absoluteFilePath(fileName));
                        QObject *plugin = loader.instance();
                        // TODO: load actually a factory as plugin

                        qCDebug(KirigamiLog) << "Loading style plugin from" << dir.absoluteFilePath(fileName);

                        KirigamiPluginFactory *factory = qobject_cast<KirigamiPluginFactory *>(plugin);
                        if (factory) {
                            pluginFactory = factory;
                            break;
                        }
                    }
#ifdef Q_OS_ANDROID
                }
#endif
            }

            // Ensure we only load the first plugin from the first plugin location.
            // If we do not break here, we may end up loading a different plugin
            // in place of the first one.
            if (pluginFactory) {
                break;
            }
        }
#endif
    }

    return pluginFactory;
}

KirigamiPluginFactoryV2::KirigamiPluginFactoryV2(QObject *parent)
    : KirigamiPluginFactory(parent)
{
}

KirigamiPluginFactoryV2::~KirigamiPluginFactoryV2() = default;
}

#include "moc_kirigamipluginfactory.cpp"
