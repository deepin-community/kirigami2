/*
 *  SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "shadowedtexturematerial.h"

#include <QOpenGLContext>

QSGMaterialType ShadowedTextureMaterial::staticType;

ShadowedTextureMaterial::ShadowedTextureMaterial()
    : ShadowedRectangleMaterial()
{
    setFlag(QSGMaterial::Blending, true);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QSGMaterialShader *ShadowedTextureMaterial::createShader() const
#else
QSGMaterialShader *ShadowedTextureMaterial::createShader(QSGRendererInterface::RenderMode) const
#endif
{
    return new ShadowedTextureShader{shaderType};
}

QSGMaterialType *ShadowedTextureMaterial::type() const
{
    return &staticType;
}

int ShadowedTextureMaterial::compare(const QSGMaterial *other) const
{
    auto material = static_cast<const ShadowedTextureMaterial *>(other);

    auto result = ShadowedRectangleMaterial::compare(other);
    if (result == 0) {
        if (material->textureSource == textureSource) {
            return 0;
        } else {
            return (material->textureSource < textureSource) ? 1 : -1;
        }
    }

    return result;
}

ShadowedTextureShader::ShadowedTextureShader(ShadowedRectangleMaterial::ShaderType shaderType)
    : ShadowedRectangleShader(shaderType)
{
    setShader(shaderType, QStringLiteral("shadowedtexture"));
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void ShadowedTextureShader::initialize()
{
    ShadowedRectangleShader::initialize();
    program()->setUniformValue("textureSource", 0);
}

void ShadowedTextureShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    ShadowedRectangleShader::updateState(state, newMaterial, oldMaterial);

    auto texture = static_cast<ShadowedTextureMaterial *>(newMaterial)->textureSource;
    if (texture) {
        texture->bind();
    }
}
#else
void ShadowedTextureShader::updateSampledImage(QSGMaterialShader::RenderState &state,
                                               int binding,
                                               QSGTexture **texture,
                                               QSGMaterial *newMaterial,
                                               QSGMaterial *oldMaterial)
{
    Q_UNUSED(state);
    Q_UNUSED(oldMaterial);
    if (binding == 1) {
        *texture = static_cast<ShadowedTextureMaterial *>(newMaterial)->textureSource;
    }
}
#endif
