# SPDX-FileCopyrightText: 2022 Volker Krause <vkrause@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

if (QT_MAJOR_VERSION EQUAL "6")
    qt6_add_shaders(KirigamiPlugin "shaders6"
        BATCHABLE
        PRECOMPILE
        OPTIMIZED
        PREFIX "/org/kde/kirigami/shaders/"
        FILES
            shadowedrectangle.vert
            shadowedrectangle.frag
            shadowedrectangle_lowpower.frag
            shadowedborderrectangle.frag
            shadowedborderrectangle_lowpower.frag
            shadowedtexture.frag
            shadowedtexture_lowpower.frag
            shadowedbordertexture.frag
            shadowedbordertexture_lowpower.frag
    )
endif()
