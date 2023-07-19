/*
 *  SPDX-FileCopyrightText: 2020 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

#include "shadowedborderrectanglematerial.h"

#include <QOpenGLContext>

QSGMaterialType ShadowedBorderRectangleMaterial::staticType;

ShadowedBorderRectangleMaterial::ShadowedBorderRectangleMaterial()
{
    setFlag(QSGMaterial::Blending, true);
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QSGMaterialShader *ShadowedBorderRectangleMaterial::createShader() const
#else
QSGMaterialShader *ShadowedBorderRectangleMaterial::createShader(QSGRendererInterface::RenderMode) const
#endif
{
    return new ShadowedBorderRectangleShader{shaderType};
}

QSGMaterialType *ShadowedBorderRectangleMaterial::type() const
{
    return &staticType;
}

int ShadowedBorderRectangleMaterial::compare(const QSGMaterial *other) const
{
    auto material = static_cast<const ShadowedBorderRectangleMaterial *>(other);

    auto result = ShadowedRectangleMaterial::compare(other);
    /* clang-format off */
    if (result == 0
        && material->borderColor == borderColor
        && qFuzzyCompare(material->borderWidth, borderWidth)) { /* clang-format on */
        return 0;
    }

    return result;
}

ShadowedBorderRectangleShader::ShadowedBorderRectangleShader(ShadowedRectangleMaterial::ShaderType shaderType)
    : ShadowedRectangleShader(shaderType)
{
    setShader(shaderType, QStringLiteral("shadowedborderrectangle"));
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void ShadowedBorderRectangleShader::initialize()
{
    ShadowedRectangleShader::initialize();
    m_borderWidthLocation = program()->uniformLocation("borderWidth");
    m_borderColorLocation = program()->uniformLocation("borderColor");
}

void ShadowedBorderRectangleShader::updateState(const QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    ShadowedRectangleShader::updateState(state, newMaterial, oldMaterial);

    auto p = program();

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0 || state.isCachedMaterialDataDirty()) {
        auto material = static_cast<ShadowedBorderRectangleMaterial *>(newMaterial);
        p->setUniformValue(m_borderWidthLocation, material->borderWidth);
        p->setUniformValue(m_borderColorLocation, material->borderColor);
    }
}
#else
bool ShadowedBorderRectangleShader::updateUniformData(QSGMaterialShader::RenderState &state, QSGMaterial *newMaterial, QSGMaterial *oldMaterial)
{
    bool changed = ShadowedRectangleShader::updateUniformData(state, newMaterial, oldMaterial);
    QByteArray *buf = state.uniformData();
    Q_ASSERT(buf->size() >= 160);

    if (!oldMaterial || newMaterial->compare(oldMaterial) != 0) {
        const auto material = static_cast<ShadowedBorderRectangleMaterial *>(newMaterial);
        memcpy(buf->data() + 136, &material->borderWidth, 8);
        float c[4];
        material->borderColor.getRgbF(&c[0], &c[1], &c[2], &c[3]);
        memcpy(buf->data() + 144, c, 16);
        changed = true;
    }

    return changed;
}
#endif
