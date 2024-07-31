// Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef ASSIMPHELPERSEX_H
#define ASSIMPHELPERSEX_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// ASSIMP INCLUDES
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <QtCore/QIODevice>
#include <QSet>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
namespace AssimpHelperEx {

//CUSTOM FILE STREAM
class AssimpIOStreamEx : public Assimp::IOStream
{
public :
    AssimpIOStreamEx(QIODevice *device);
    ~AssimpIOStreamEx();

    size_t Read(void *pvBuffer, size_t pSize, size_t pCount) override;
    size_t Write(const void *pvBuffer, size_t pSize, size_t pCount) override;
    aiReturn Seek(size_t pOffset, aiOrigin pOrigin) override;
    size_t Tell() const override;
    size_t FileSize() const override;
    void Flush() override;

private:
    QIODevice *const m_device;
};

//CUSTOM FILE IMPORTER TO HANDLE QT RESOURCES WITHIN ASSIMP
class AssimpIOSystemEx : public Assimp::IOSystem
{
public :
    bool Exists(const char *pFile) const override;
    char getOsSeparator() const override;
    Assimp::IOStream *Open(const char *pFile, const char *pMode) override;
    void Close(Assimp::IOStream *pFile) override;
};

class TextureDataByStb
{
public:
    QImage getStbTextureImage(const unsigned char* assimpTex, int len);
	void clearStbTextureData();
private:
	QSet<unsigned char*> m_stbDataSets;
};

} // namespace AssimpHelper
} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // ASSIMPHELPERS_H
