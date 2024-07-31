// Copyright (C) 2016 The Qt Company Ltd and/or its subsidiary(-ies).
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "assimpimporterex.h"

#include <Qt3DRender/private/qsceneimportplugin_p.h>

QT_BEGIN_NAMESPACE

class AssimpSceneImportExPlugin : public Qt3DRender::QSceneImportPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QSceneImportFactoryInterface_iid FILE "assimpex.json")

    Qt3DRender::QSceneImporter *create(const QString &key, const QStringList &paramList) override
    {
        Q_UNUSED(key);
        Q_UNUSED(paramList);
        return new Qt3DRender::AssimpImporterEx();
    }
};

QT_END_NAMESPACE

#include "main.moc"
