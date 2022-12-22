/*
 *  SPDX-FileCopyrightText: 2020 Jonah Brüchert <jbb@kaidan.im>
 *  SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "units.h"

#include <QFont>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QStyleHints>

#include <chrono>
#include <cmath>

#include "loggingcategory.h"

namespace Kirigami {
using clock = std::chrono::steady_clock;

const clock::duration rateLimit = std::chrono::seconds(1);

/* Print a deprecation warning that is rate limited to only display once in
 * every time period as determined by rateLimit. We keep track of how often this
 * is called and display that if it is larger than 0.
 *
 * This is done to prevent flooding the logs with "X is deprecated" messages
 * that are all the same and don't provide any new information after the first.
 */
void rateLimitWarning(const char *method, const char *since, const char *message)
{
    static QMap<QString, QPair<clock::time_point, int>> messages;

    auto methodString = QString::fromUtf8(method);

    if (!messages.contains(methodString)) {
        messages.insert(methodString, qMakePair(clock::time_point{}, 0));
    }

    auto entry = messages.value(methodString);
    if (clock::now() - entry.first < rateLimit) {
        messages[methodString].second += 1;
        return;
    }

    qCWarning(KirigamiLog).nospace() << method << " is deprecated (since " << since << "): " << message;

    if (entry.second > 0) {
        qCWarning(KirigamiLog) << "Previous message repeats" << entry.second << "times.";
    }

    messages[methodString] = qMakePair(clock::now(), 0);
}

class UnitsPrivate
{
    Q_DISABLE_COPY(UnitsPrivate)

public:
    explicit UnitsPrivate(Units *units)
#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
        : qmlFontMetrics(nullptr)
#endif
        // Cache font so we don't have to go through QVariant and property every time
        , fontMetrics(QFontMetricsF(QGuiApplication::font()))
        , gridUnit(fontMetrics.height())
        , smallSpacing(std::floor(gridUnit / 4))
        , largeSpacing(smallSpacing * 2)
        , veryLongDuration(400)
        , longDuration(200)
        , shortDuration(100)
        , veryShortDuration(50)
        , humanMoment(2000)
        , toolTipDelay(700)
#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
        , wheelScrollLines(QGuiApplication::styleHints()->wheelScrollLines())
#endif
        , iconSizes(new IconSizes(units))
    {
    }

    // Only stored for QML API compatiblity
    // TODO KF6 drop
#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
    QObject *qmlFontMetrics;
#endif

    // Font metrics used for Units.
    // TextMetrics uses QFontMetricsF internally, so this should do the same
    QFontMetricsF fontMetrics;

    // units
    int gridUnit;
    int smallSpacing;
    int largeSpacing;

    // durations
    int veryLongDuration;
    int longDuration;
    int shortDuration;
    int veryShortDuration;
    int humanMoment;
    int toolTipDelay;

#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
    int wheelScrollLines;
#endif

    IconSizes *const iconSizes;

    // To prevent overriding custom set units if the font changes
    bool customUnitsSet = false;
    bool customWheelScrollLinesSet = false;

#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
    QObject *createQmlFontMetrics(QQmlEngine *engine)
    {
        QQmlComponent component(engine);
        component.setData(QByteArrayLiteral(
            "import QtQuick 2.14\n"
            "import org.kde.kirigami 2.0\n"
            "FontMetrics {\n"
            "	function roundedIconSize(size) {\n"
            R"(		console.warn("Units.fontMetrics.roundedIconSize is deprecated, use Units.iconSizes.roundedIconSize instead.");)"
            "		return Units.iconSizes.roundedIconSize(size)\n"
            "	}\n"
            "}\n"
        ), QUrl(QStringLiteral("units.cpp")));

        return component.create();
    }
#endif
};

Units::~Units() = default;

Units::Units(QObject *parent)
    : QObject(parent)
    , d(std::make_unique<UnitsPrivate>(this))
{
    connect(QGuiApplication::styleHints(), &QStyleHints::wheelScrollLinesChanged, this, [this](int scrollLines) {
        if (d->customWheelScrollLinesSet) {
            return;
        }

        setWheelScrollLines(scrollLines);
    });
    connect(qGuiApp, &QGuiApplication::fontChanged, this, [this](const QFont &font) {
        d->fontMetrics = QFontMetricsF(font);

        if (d->customUnitsSet) {
            return;
        }

        d->gridUnit = d->fontMetrics.height();
        Q_EMIT gridUnitChanged();
        d->smallSpacing = std::floor(d->gridUnit / 4);
        Q_EMIT smallSpacingChanged();
        d->largeSpacing = d->smallSpacing * 2;
        Q_EMIT largeSpacingChanged();
        Q_EMIT d->iconSizes->sizeForLabelsChanged();
    });
}

qreal Units::devicePixelRatio() const
{
    rateLimitWarning("Units.devicePixelRatio", "5.86", "This returns 1 when using Qt HiDPI scaling.");
    const int pixelSize = QGuiApplication::font().pixelSize();
    const qreal pointSize = QGuiApplication::font().pointSize();

    return std::fmax(1, (pixelSize * 0.75 / pointSize));
}

int Units::gridUnit() const
{
    return d->gridUnit;
}

void Kirigami::Units::setGridUnit(int size)
{
    if (d->gridUnit == size) {
        return;
    }

    d->gridUnit = size;
    d->customUnitsSet = true;
    Q_EMIT gridUnitChanged();
}

int Units::smallSpacing() const
{
    return d->smallSpacing;
}

void Kirigami::Units::setSmallSpacing(int size)
{
    if (d->smallSpacing == size) {
        return;
    }

    d->smallSpacing = size;
    d->customUnitsSet = true;
    Q_EMIT smallSpacingChanged();
}

int Units::largeSpacing() const
{
    return d->largeSpacing;
}

void Kirigami::Units::setLargeSpacing(int size)
{
    if (d->largeSpacing) {
        return;
    }

    d->largeSpacing = size;
    d->customUnitsSet = true;
    Q_EMIT largeSpacingChanged();
}

int Units::veryLongDuration() const
{
    return d->veryLongDuration;
}

void Units::setVeryLongDuration(int duration)
{
    if (d->veryLongDuration == duration) {
        return;
    }

    d->veryLongDuration = duration;
    Q_EMIT veryLongDurationChanged();
}

int Units::longDuration() const
{
    return d->longDuration;
}

void Units::setLongDuration(int duration)
{
    if (d->longDuration == duration) {
        return;
    }

    d->longDuration = duration;
    Q_EMIT longDurationChanged();
}

int Units::shortDuration() const
{
    return d->shortDuration;
}

void Units::setShortDuration(int duration)
{
    if (d->shortDuration == duration) {
        return;
    }

    d->shortDuration = duration;
    Q_EMIT shortDurationChanged();
}

int Units::veryShortDuration() const
{
    return d->veryLongDuration;
}

void Units::setVeryShortDuration(int duration)
{
    if (d->veryLongDuration == duration) {
        return;
    }

    d->veryLongDuration = duration;
    Q_EMIT veryShortDurationChanged();
}

int Units::humanMoment() const
{
    return d->humanMoment;
}

void Units::setHumanMoment(int duration)
{
    if (d->humanMoment == duration) {
        return;
    }

    d->humanMoment = duration;
    Q_EMIT humanMomentChanged();
}

int Units::toolTipDelay() const
{
    return d->toolTipDelay;
}

void Units::setToolTipDelay(int delay)
{
    if (d->toolTipDelay == delay) {
        return;
    }

    d->toolTipDelay = delay;
    Q_EMIT toolTipDelayChanged();
}

#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
int Units::wheelScrollLines() const
{
    rateLimitWarning("Units.wheelScrollLines", "5.86", "Use Qt.styleHints.wheelScrollLines instead");
    return d->wheelScrollLines;
}

void Units::setWheelScrollLines(int lines)
{
    if (d->wheelScrollLines == lines) {
        return;
    }

    d->wheelScrollLines = lines;
    d->customWheelScrollLinesSet = true;
    Q_EMIT wheelScrollLinesChanged();
}
#endif

IconSizes *Units::iconSizes() const
{
    return d->iconSizes;
}

#if KIRIGAMI2_BUILD_DEPRECATED_SINCE(5, 86)
QObject *Units::fontMetrics() const
{
    rateLimitWarning("Units.fontMetrics", "5.86", "Create your own FontMetrics object instead.");
    if (!d->qmlFontMetrics) {
        d->qmlFontMetrics = d->createQmlFontMetrics(qmlEngine(this));
    }
    return d->qmlFontMetrics;
}
#endif

IconSizes::IconSizes(Units *units)
    : QObject(units)
    , m_units(units)
{
}

int IconSizes::roundedIconSize(int size) const
{
    if (size < 16) {
        return size;
    }

    if (size < 22) {
        return 16;
    }

    if (size < 32) {
        return 22;
    }

    if (size < 48) {
        return 32;
    }

    if (size < 64) {
        return 48;
    }

    return size;
}

int IconSizes::sizeForLabels() const
{
    // gridUnit is the height of textMetrics
    return roundedIconSize(m_units->d->fontMetrics.height());
}

int IconSizes::small() const
{
    return 16;
}

int IconSizes::smallMedium() const
{
    return 22;
}

int IconSizes::medium() const
{
    return 32;
}

int IconSizes::large() const
{
    return 48;
}

int IconSizes::huge() const
{
    return 64;
}

int IconSizes::enormous() const
{
    return 128;
}

}
