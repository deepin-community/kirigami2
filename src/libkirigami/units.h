/*
 *  SPDX-FileCopyrightText: 2021 Jonah Brüchert <jbb@kaidan.im>
 *  SPDX-FileCopyrightText: 2015 Marco Martin <mart@kde.org>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QObject>
#include <memory>

#include "kirigami2_export.h"

class QQmlEngine;

namespace Kirigami {
class Units;
class UnitsPrivate;

/**
 * @class IconSizes units.h <Kirigami/Units>
 *
 * Provides access to platform-dependent icon sizing
 */
class KIRIGAMI2_EXPORT IconSizes : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int sizeForLabels READ sizeForLabels NOTIFY sizeForLabelsChanged)
    Q_PROPERTY(int small READ small NOTIFY smallChanged)
    Q_PROPERTY(int smallMedium READ smallMedium NOTIFY smallMediumChanged)
    Q_PROPERTY(int medium READ medium NOTIFY mediumChanged)
    Q_PROPERTY(int large READ large NOTIFY largeChanged)
    Q_PROPERTY(int huge READ huge NOTIFY hugeChanged)
    Q_PROPERTY(int enormous READ enormous NOTIFY enormousChanged)

public:
    IconSizes(Units *units);

    int sizeForLabels() const;
    int small() const;
    int smallMedium() const;
    int medium() const;
    int large() const;
    int huge() const;
    int enormous() const;

    Q_INVOKABLE int roundedIconSize(int size) const;

private:
    float iconScaleFactor() const;

    Units *m_units;

Q_SIGNALS:
    void sizeForLabelsChanged();
    void smallChanged();
    void smallMediumChanged();
    void mediumChanged();
    void largeChanged();
    void hugeChanged();
    void enormousChanged();
};

/**
 * @class Units units.h <Kirigami/Units>
 *
 * A set of values to define semantically sizes and durations.
 */
class KIRIGAMI2_EXPORT Units : public QObject
{
    Q_OBJECT

    friend class IconSizes;

    /**
     * The fundamental unit of space that should be used for sizes, expressed in pixels.
     * Given the screen has an accurate DPI settings, it corresponds to the height of
     * the font's boundingRect.
     */
    Q_PROPERTY(int gridUnit READ gridUnit WRITE setGridUnit NOTIFY gridUnitChanged)

    /**
     * units.iconSizes provides access to platform-dependent icon sizing
     *
     * The icon sizes provided are normalized for different DPI, so icons
     * will scale depending on the DPI.
     *
     * * sizeForLabels (the largest icon size that fits within fontMetrics.height) @since 5.80 @since org.kde.kirigami 2.16
     * * small
     * * smallMedium
     * * medium
     * * large
     * * huge
     * * enormous
     */
    Q_PROPERTY(IconSizes *iconSizes READ iconSizes CONSTANT)

    /**
     * This property holds the amount of spacing that should be used between smaller UI elements,
     * such as a small icon and a label in a button.
     * Internally, this size depends on the size of the default font as rendered on the screen,
     * so it takes user-configured font size and DPI into account.
     */
    Q_PROPERTY(int smallSpacing READ smallSpacing WRITE setSmallSpacing NOTIFY smallSpacingChanged)

    /**
     * This property holds the amount of spacing that should be used between medium UI elements,
     * such as buttons and text fields in a toolbar.
     * Internally, this size depends on the size of the default font as rendered on the screen,
     * so it takes user-configured font size and DPI into account.
     */
    Q_PROPERTY(int mediumSpacing READ mediumSpacing WRITE setMediumSpacing NOTIFY mediumSpacingChanged)

    /**
     * This property holds the amount of spacing that should be used between bigger UI elements,
     * such as a large icon and a heading in a card.
     * Internally, this size depends on the size of the default font as rendered on the screen,
     * so it takes user-configured font size and DPI into account.
     */
    Q_PROPERTY(int largeSpacing READ largeSpacing WRITE setLargeSpacing NOTIFY largeSpacingChanged)

    /**
     * The ratio between physical and device-independent pixels. This value does not depend on the \
     * size of the configured font. If you want to take font sizes into account when scaling elements,
     * use theme.mSize(theme.defaultFont), units.smallSpacing and units.largeSpacing.
     * The devicePixelRatio follows the definition of "device independent pixel" by Microsoft.
     *
     * @deprecated since 5.86. When using Qt's high DPI scaling, all sizes are
     * considered to be device-independent pixels, so this will simply return 1.
     */
    Q_PROPERTY(qreal devicePixelRatio READ devicePixelRatio NOTIFY devicePixelRatioChanged)

    /**
     * units.veryLongDuration should be used for specialty animations that benefit
     * from being even longer than longDuration.
     */
    Q_PROPERTY(int veryLongDuration READ veryLongDuration WRITE setVeryLongDuration NOTIFY veryLongDurationChanged)

    /**
     * units.longDuration should be used for longer, screen-covering animations, for opening and
     * closing of dialogs and other "not too small" animations
     */
    Q_PROPERTY(int longDuration READ longDuration WRITE setLongDuration NOTIFY longDurationChanged)

    /**
     * units.shortDuration should be used for short animations, such as accentuating a UI event,
     * hover events, etc..
     */
    Q_PROPERTY(int shortDuration READ shortDuration WRITE setShortDuration NOTIFY shortDurationChanged)

    /**
     * units.veryShortDuration should be used for elements that should have a hint of smoothness,
     * but otherwise animate near instantly.
     */
    Q_PROPERTY(int veryShortDuration READ veryShortDuration WRITE setVeryShortDuration NOTIFY veryShortDurationChanged)

    /**
     * Time in milliseconds equivalent to the theoretical human moment, which can be used
     * to determine whether how long to wait until the user should be informed of something,
     * or can be used as the limit for how long something should wait before being
     * automatically initiated.
     *
     * Some examples:
     *
     * - When the user types text in a search field, wait no longer than this duration after
     *   the user completes typing before starting the search
     * - When loading data which would commonly arrive rapidly enough to not require interaction,
     *   wait this long before showing a spinner
     *
     * This might seem an arbitrary number, but given the psychological effect that three
     * seconds seems to be what humans consider a moment (and in the case of waiting for
     * something to happen, a moment is that time when you think "this is taking a bit long,
     * isn't it?"), the idea is to postpone for just before such a conceptual moment. The reason
     * for the two seconds, rather than three, is to function as a middle ground: Not long enough
     * that the user would think that something has taken too long, for also not so fast as to
     * happen too soon.
     *
     * See also
     * https://www.psychologytoday.com/blog/all-about-addiction/201101/tick-tock-tick-hugs-and-life-in-3-second-intervals
     * (the actual paper is hidden behind an academic paywall and consequently not readily
     * available to us, so the source will have to be the blog entry above)
     *
     * \note This should __not__ be used as an animation duration, as it is deliberately not scaled according
     * to the animation settings. This is specifically for determining when something has taken too long and
     * the user should expect some kind of feedback. See veryShortDuration, shortDuration, longDuration, and
     * veryLongDuration for animation duration choices.
     *
     * @since 5.81
     * @since org.kde.kirigami 2.16
     */
    Q_PROPERTY(int humanMoment READ humanMoment WRITE setHumanMoment NOTIFY humanMomentChanged)

    /**
     * time in ms by which the display of tooltips will be delayed.
     *
     * @sa ToolTip.delay property
     */
    Q_PROPERTY(int toolTipDelay READ toolTipDelay WRITE setToolTipDelay NOTIFY toolTipDelayChanged)

#if KIRIGAMI2_ENABLE_DEPRECATED_SINCE(5, 86)
    /**
     * How much the mouse scroll wheel scrolls, expressed in lines of text.
     * Note: this is strictly for classical mouse wheels, touchpads 2 figer scrolling won't be affected
     */
    Q_PROPERTY(int wheelScrollLines READ wheelScrollLines NOTIFY wheelScrollLinesChanged)
#endif

#if KIRIGAMI2_ENABLE_DEPRECATED_SINCE(5, 86)
    /**
     * metrics used by the default font
     *
     * @deprecated since 5.86.0, Create your own TextMetrics object if needed.
     * For the roundedIconSize function, use Units.iconSizes.roundedIconSize instead
     */
    Q_PROPERTY(QObject *fontMetrics READ fontMetrics CONSTANT)
#endif

    Q_PROPERTY(int maximumInteger READ maximumInteger CONSTANT)

public:
    explicit Units(QObject *parent = nullptr);
    ~Units() override;

    int gridUnit() const;
    void setGridUnit(int size);

    int smallSpacing() const;
    void setSmallSpacing(int size);

    int mediumSpacing() const;
    void setMediumSpacing(int size);

    int largeSpacing() const;
    void setLargeSpacing(int size);

#if KIRIGAMI2_ENABLE_DEPRECATED_SINCE(5, 86)
    // TODO KF6 remove
    KIRIGAMI2_DEPRECATED_VERSION(5, 86, "When using Qt scaling, this would return a value of 1")
    qreal devicePixelRatio() const;
#endif

    int veryLongDuration() const;
    void setVeryLongDuration(int duration);

    int longDuration() const;
    void setLongDuration(int duration);

    int shortDuration() const;
    void setShortDuration(int duration);

    int veryShortDuration() const;
    void setVeryShortDuration(int duration);

    int humanMoment() const;
    void setHumanMoment(int duration);

    int toolTipDelay() const;
    void setToolTipDelay(int delay);

#if KIRIGAMI2_ENABLE_DEPRECATED_SINCE(5, 86)
    // TODO KF6 remove
    KIRIGAMI2_DEPRECATED_VERSION(5, 86, "Use Qt.styleHints.wheelScrollLines instead")
    int wheelScrollLines() const;
    void setWheelScrollLines(int lines);
#endif

    IconSizes *iconSizes() const;

    int maximumInteger() const;

Q_SIGNALS:
    void gridUnitChanged();
    void smallSpacingChanged();
    void mediumSpacingChanged();
    void largeSpacingChanged();
    void devicePixelRatioChanged();
    void veryLongDurationChanged();
    void longDurationChanged();
    void shortDurationChanged();
    void veryShortDurationChanged();
    void humanMomentChanged();
    void toolTipDelayChanged();
    void wheelScrollLinesChanged();

private:
#if KIRIGAMI2_ENABLE_DEPRECATED_SINCE(5, 86)
    QObject *fontMetrics() const;
#endif

    std::unique_ptr<UnitsPrivate> d;
};

}
