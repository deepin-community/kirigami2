/*
 *  SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QQuickItem>
#include <memory>

class PaintedRectangleItem;

/**
 * Grouped property for rectangle border.
 */
class BorderGroup : public QObject
{
    Q_OBJECT
    /**
     * The width of the border in pixels.
     *
     * Default is 0.
     */
    Q_PROPERTY(qreal width READ width WRITE setWidth NOTIFY changed)
    /**
     * The color of the border.
     *
     * Full RGBA colors are supported. The default is fully opaque black.
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)

public:
    explicit BorderGroup(QObject *parent = nullptr);

    qreal width() const;
    void setWidth(qreal newWidth);

    QColor color() const;
    void setColor(const QColor &newColor);

    Q_SIGNAL void changed();

    inline bool isEnabled() const
    {
        return !qFuzzyIsNull(m_width);
    }

private:
    qreal m_width = 0.0;
    QColor m_color = Qt::black;
};

/**
 * Grouped property for rectangle shadow.
 */
class ShadowGroup : public QObject
{
    Q_OBJECT
    /**
     * The size of the shadow.
     *
     * This is the approximate size of the shadow in pixels. However, due to falloff
     * the actual shadow size can differ. The default is 0, which means no shadow will
     * be rendered.
     */
    Q_PROPERTY(qreal size READ size WRITE setSize NOTIFY changed)
    /**
     * Offset of the shadow on the X axis.
     *
     * In pixels. The default is 0.
     */
    Q_PROPERTY(qreal xOffset READ xOffset WRITE setXOffset NOTIFY changed)
    /**
     * Offset of the shadow on the Y axis.
     *
     * In pixels. The default is 0.
     */
    Q_PROPERTY(qreal yOffset READ yOffset WRITE setYOffset NOTIFY changed)
    /**
     * The color of the shadow.
     *
     * Full RGBA colors are supported. The default is fully opaque black.
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)

public:
    explicit ShadowGroup(QObject *parent = nullptr);

    qreal size() const;
    void setSize(qreal newSize);

    qreal xOffset() const;
    void setXOffset(qreal newXOffset);

    qreal yOffset() const;
    void setYOffset(qreal newYOffset);

    QColor color() const;
    void setColor(const QColor &newShadowColor);

    Q_SIGNAL void changed();

private:
    qreal m_size = 0.0;
    qreal m_xOffset = 0.0;
    qreal m_yOffset = 0.0;
    QColor m_color = Qt::black;
};

/**
 * Grouped property for corner radius.
 */
class CornersGroup : public QObject
{
    Q_OBJECT
    /**
     * The radius of the top-left corner.
     *
     * In pixels. Defaults to -1, which indicates this value should not be used.
     */
    Q_PROPERTY(qreal topLeftRadius READ topLeft WRITE setTopLeft NOTIFY changed)
    /**
     * The radius of the top-right corner.
     *
     * In pixels. Defaults to -1, which indicates this value should not be used.
     */
    Q_PROPERTY(qreal topRightRadius READ topRight WRITE setTopRight NOTIFY changed)
    /**
     * The radius of the bottom-left corner.
     *
     * In pixels. Defaults to -1, which indicates this value should not be used.
     */
    Q_PROPERTY(qreal bottomLeftRadius READ bottomLeft WRITE setBottomLeft NOTIFY changed)
    /**
     * The radius of the bottom-right corner.
     *
     * In pixels. Defaults to -1, which indicates this value should not be used.
     */
    Q_PROPERTY(qreal bottomRightRadius READ bottomRight WRITE setBottomRight NOTIFY changed)

public:
    explicit CornersGroup(QObject *parent = nullptr);

    qreal topLeft() const;
    void setTopLeft(qreal newTopLeft);

    qreal topRight() const;
    void setTopRight(qreal newTopRight);

    qreal bottomLeft() const;
    void setBottomLeft(qreal newBottomLeft);

    qreal bottomRight() const;
    void setBottomRight(qreal newBottomRight);

    Q_SIGNAL void changed();

    QVector4D toVector4D(float all) const;

private:
    float m_topLeft = -1.0;
    float m_topRight = -1.0;
    float m_bottomLeft = -1.0;
    float m_bottomRight = -1.0;
};

/**
 * A rectangle with a shadow.
 *
 * This item will render a rectangle, with a shadow below it. The rendering is done
 * using distance fields, which provide greatly improved performance. The shadow is
 * rendered outside of the item's bounds, so the item's width and height are the
 * rectangle's width and height.
 *
 * @since 5.69 / 2.12
 */
class ShadowedRectangle : public QQuickItem
{
    Q_OBJECT
    /**
     * Corner radius of the rectangle.
     *
     * This is the amount of rounding to apply to all of the rectangle's
     * corners, in pixels. Individual corners can have a different radius, see
     * \property corners.
     *
     * The default is 0.
     */
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    /**
     * The color of the rectangle.
     *
     * Full RGBA colors are supported. The default is fully opaque white.
     */
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    /**
     * Border properties.
     *
     * \sa BorderGroup
     */
    Q_PROPERTY(BorderGroup *border READ border CONSTANT)
    /**
     * Shadow properties.
     *
     * \sa ShadowGroup
     */
    Q_PROPERTY(ShadowGroup *shadow READ shadow CONSTANT)
    /**
     * Corner radius.
     *
     * Note that the values from this group override \property radius for the
     * corner they affect.
     *
     * \sa CornerGroup
     */
    Q_PROPERTY(CornersGroup *corners READ corners CONSTANT)

    /**
     * Render type of the rectangle and shadow.
     *
     * The default is Auto.
     */
    Q_PROPERTY(RenderType renderType READ renderType WRITE setRenderType CONSTANT)

    Q_PROPERTY(bool softwareRendering READ isSoftwareRendering NOTIFY softwareRenderingChanged)
public:
    ShadowedRectangle(QQuickItem *parent = nullptr);
    ~ShadowedRectangle() override;

    /**
     * Available rendering types for ShadowedRectangle.
     */
    enum RenderType {
        /**
         * Automatically determine the optimal rendering type.
         * This will use the highest rendering quality possible, falling back to
         * lower quality if the hardware doesn't support it. It will use software
         * rendering if the QtQuick scene graph is set to use software rendering.
         */
        Auto,
        /**
         * Use the highest rendering quality possible, even if the hardware might
         * not be able to handle it normally.
         */
        HighQuality,
        /**
         * Use the lowest rendering quality, even if the hardware could handle 
         * higher quality rendering. This might result in certain effects being
         * omitted, like shadows.
         */
        LowQuality,
        /**
         * Always use software rendering for this rectangle.
         * Software rendering is intended as a fallback when the QtQuick scene 
         * graph is configured to use software rendering. It will result in 
         * a number of missing features, like shadows and multiple corner radii.
         */
        Software 
    };
    Q_ENUM(RenderType)

    BorderGroup *border() const;
    ShadowGroup *shadow() const;
    CornersGroup *corners() const;

    qreal radius() const;
    void setRadius(qreal newRadius);
    Q_SIGNAL void radiusChanged();

    QColor color() const;
    void setColor(const QColor &newColor);
    Q_SIGNAL void colorChanged();

    RenderType renderType() const;
    void setRenderType(RenderType renderType);
    Q_SIGNAL void renderTypeChanged();

    void componentComplete() override;

    bool isSoftwareRendering() const;

Q_SIGNALS:
    void softwareRenderingChanged();

protected:
    PaintedRectangleItem *softwareItem() const;
    void itemChange(QQuickItem::ItemChange change, const QQuickItem::ItemChangeData &value) override;
    QSGNode *updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data) override;

private:
    void checkSoftwareItem();
    const std::unique_ptr<BorderGroup> m_border;
    const std::unique_ptr<ShadowGroup> m_shadow;
    const std::unique_ptr<CornersGroup> m_corners;
    qreal m_radius = 0.0;
    QColor m_color = Qt::white;
    RenderType m_renderType = RenderType::Auto;
    PaintedRectangleItem *m_softwareItem = nullptr;
};
