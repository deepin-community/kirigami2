/*
 *  SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#pragma once

#include <QSGTexture>

#include "shadowedrectanglematerial.h"

/**
 * A material rendering a rectangle with a shadow.
 *
 * This material uses a distance field shader to render a rectangle with a
 * shadow below it, optionally with rounded corners.
 */
class ShadowedTextureMaterial : public ShadowedRectangleMaterial
{
public:
    ShadowedTextureMaterial();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QSGMaterialShader *createShader() const override;
#else
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode) const override;
#endif
    QSGMaterialType *type() const override;
    int compare(const QSGMaterial *other) const override;

    QSGTexture *textureSource = nullptr;

    static QSGMaterialType staticType;
};

class ShadowedTextureShader : public ShadowedRectangleShader
{
public:
    ShadowedTextureShader(ShadowedRectangleMaterial::ShaderType shaderType);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void initialize() override;
    void updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
#else
    void
    updateSampledImage(QSGMaterialShader::RenderState &state, int binding, QSGTexture **texture, QSGMaterial *newMaterial, QSGMaterial *oldMaterial) override;
#endif
};
