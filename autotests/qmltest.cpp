/*
 *  SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include <QQmlEngine>
#include <QtQuickTest>

#ifdef STATIC_MODULE
#include "kirigamiplugin.h"
Q_IMPORT_PLUGIN(KirigamiPlugin)
#endif

class KirigamiSetup : public QObject
{
    Q_OBJECT

public:
    KirigamiSetup()
    {
    }

public Q_SLOTS:
    void qmlEngineAvailable(QQmlEngine *engine)
    {
#ifdef STATIC_MODULE
        KirigamiPlugin::getInstance().registerTypes(engine);
#else
        Q_UNUSED(engine)
#endif
    }
};

QUICK_TEST_MAIN_WITH_SETUP(Kirigami, KirigamiSetup)

#include "qmltest.moc"
