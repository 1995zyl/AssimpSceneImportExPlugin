// Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include "assimphelpersex.h"

#include <QtCore/QFileDevice>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QImage>
#include <memory>
#include <stb_image.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
namespace AssimpHelperEx {
namespace {
	struct TextureByStb
	{
		int m_width;
		int m_height;
		int m_channel;
		unsigned char* m_data;
	};
}
/*!
 *  \class Qt3DRender::AssimpHelper::AssimpIOStream
 *
 *  \internal
 *
 *  \brief Provides a custom file stream class to be used by AssimpIOSystem.
 *
 */

/*!
 *  Builds a new AssimpIOStreamEx instance.
 */
AssimpIOStreamEx::AssimpIOStreamEx(QIODevice *device) :
    Assimp::IOStream(),
    m_device(device)
{
    Q_ASSERT(m_device != nullptr);
}

/*!
 *  Clears an AssimpIOStreamEx instance before deletion.
 */
AssimpIOStreamEx::~AssimpIOStreamEx()
{
    // Owns m_device
    delete m_device;
}

/*!
 *  Reads at most \a pCount elements of \a pSize bytes of data into \a pvBuffer.
 *  Returns the number of bytes read or -1 if an error occurred.
 */
size_t AssimpIOStreamEx::Read(void *pvBuffer, size_t pSize, size_t pCount)
{
    qint64 readBytes = m_device->read((char *)pvBuffer, pSize * pCount);
    if (readBytes < 0)
        qWarning() << Q_FUNC_INFO << " Reading failed";
    Q_ASSERT(pSize > 0);
    return readBytes / pSize;
}


/*!
 * Writes \a pCount elements of \a pSize bytes from \a pvBuffer.
 * Returns the number of bytes written or -1 if an error occurred.
 */
size_t AssimpIOStreamEx::Write(const void *pvBuffer, size_t pSize, size_t pCount)
{
    qint64 writtenBytes = m_device->write((char *)pvBuffer, pSize * pCount);
    if (writtenBytes < 0)
        qWarning() << Q_FUNC_INFO << " Writing failed";
    return writtenBytes;
}

/*!
 * Seeks the current file descriptor to a position defined by \a pOrigin and \a pOffset
 */
aiReturn AssimpIOStreamEx::Seek(size_t pOffset, aiOrigin pOrigin)
{
    qint64 seekPos = pOffset;

    if (pOrigin == aiOrigin_CUR)
        seekPos += m_device->pos();
    else if (pOrigin == aiOrigin_END)
        seekPos += m_device->size();

    if (!m_device->seek(seekPos)) {
        qWarning() << Q_FUNC_INFO << " Seeking failed";
        return aiReturn_FAILURE;
    }
    return aiReturn_SUCCESS;
}

/*!
 * Returns the current position of the read/write cursor.
 */
size_t AssimpIOStreamEx::Tell() const
{
    return m_device->pos();
}

/*!
 * Returns the filesize;
 */
size_t AssimpIOStreamEx::FileSize() const
{
    return m_device->size();
}

/*!
 * Flushes the current device.
 */
void AssimpIOStreamEx::Flush()
{
    // QIODevice has no flush method
    if (QFileDevice *file = qobject_cast<QFileDevice *>(m_device))
        file->flush();
}

/*!
 * \class Qt3DRender::AssimpHelper::AssimpIOSystem
 *
 * \internal
 *
 * \brief Provides a custom file handler to the Assimp importer in order to handle
 * various Qt specificities (QResources ...)
 *
 */

static QIODevice::OpenMode openModeFromText(const char *name) noexcept
{
    static const struct OpenModeMapping {
        char name[2];
        ushort mode;
    } openModeMapping[] = {
        { { 'r',   0 },  QIODevice::ReadOnly  },
        { { 'r', '+' },  QIODevice::ReadWrite },
        { { 'w',   0 },  QIODevice::WriteOnly | QIODevice::Truncate },
        { { 'w', '+' },  QIODevice::ReadWrite | QIODevice::Truncate },
        { { 'a',   0 },  QIODevice::WriteOnly | QIODevice::Append },
        { { 'a', '+' },  QIODevice::ReadWrite | QIODevice::Append },
        { { 'w', 'b' },  QIODevice::WriteOnly },
        { { 'w', 't' },  QIODevice::WriteOnly | QIODevice::Text },
        { { 'r', 'b' },  QIODevice::ReadOnly  },
        { { 'r', 't' },  QIODevice::ReadOnly  | QIODevice::Text },
    };

    for (auto e : openModeMapping) {
        if (qstrncmp(e.name, name, sizeof(OpenModeMapping::name)) == 0)
            return static_cast<QIODevice::OpenMode>(e.mode);
    }
    return QIODevice::NotOpen;
}

/*!
 * Returns true if the file located at pFile exists, false otherwise.
 */
bool AssimpIOSystemEx::Exists(const char *pFile) const
{
    return QFileInfo::exists(QString::fromUtf8(pFile));
}

/*!
 * Returns the default system separator.
 */
char AssimpIOSystemEx::getOsSeparator() const
{
    return QDir::separator().toLatin1();
}

/*!
 * Opens the file located at \a pFile with the opening mode
 * specified by \a pMode.
 */
Assimp::IOStream * AssimpIOSystemEx::Open(const char *pFile, const char *pMode)
{
    const QString fileName(QString::fromUtf8(pFile));
    const QLatin1String cleanedMode = QLatin1String{pMode}.trimmed();

    if (const QIODevice::OpenMode openMode = openModeFromText(cleanedMode.data())) {
        auto file = std::make_unique<QFile>(fileName);
        if (file->open(openMode))
            return new AssimpIOStreamEx(file.release());
    }
    return nullptr;
}

/*!
 * Closes the Assimp::IOStream \a pFile.
 */
void AssimpIOSystemEx::Close(Assimp::IOStream *pFile)
{
    // Assimp::IOStream has a virtual destructor which closes the stream
    delete pFile;
}

QImage TextureDataByStb::getStbTextureImage(const unsigned char* assimpTex, int len)
{
	TextureByStb assTexStb;
    assTexStb.m_data = stbi_load_from_memory(assimpTex, len, &assTexStb.m_width, &assTexStb.m_height, &assTexStb.m_channel, 0);
	m_stbDataSets.insert(assTexStb.m_data);
    return QImage(assTexStb.m_data, assTexStb.m_width, assTexStb.m_height, QImage::Format_RGBA8888);
}

void TextureDataByStb::clearStbTextureData()
{
	for (auto it = m_stbDataSets.begin(); m_stbDataSets.end() != it; ++it)
	{
		stbi_image_free(*it);
	}
	m_stbDataSets.clear();
}

} // namespace AssimpHelper
} // namespace Qt3DRender

QT_END_NAMESPACE
