/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef STYLESELECTOR_H
#define STYLESELECTOR_H

#include <QStringList>

#include <kirigami2_export.h>

class QUrl;

namespace Kirigami
{
class KIRIGAMI2_EXPORT StyleSelector
{
public:
    static QString style();
    static QStringList styleChain();

    static QUrl componentUrl(const QString &fileName);

    static void setBaseUrl(const QUrl &baseUrl);

    static QString resolveFilePath(const QString &path);
    static QString resolveFileUrl(const QString &path);

private:
    static QUrl s_baseUrl;
    static QStringList s_styleChain;
};

}

#endif // STYLESELECTOR_H
