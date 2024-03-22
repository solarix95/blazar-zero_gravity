#include <QDebug>
#include <QDir>
#include <QStringList>
#include "bztextures.h"

//-------------------------------------------------------------------------------------------------
BzTextures::BzTextures()
{
}

//-------------------------------------------------------------------------------------------------
BzTextures::~BzTextures()
{
   reset();
}

//-------------------------------------------------------------------------------------------------
QImage BzTextures::operator [](const char *name) const
{
    return (*this)[QString::fromUtf8(name)];
}

//-------------------------------------------------------------------------------------------------
QImage BzTextures::operator[](const QString &name) const
{
    if (!mCache.contains(name))
        return QImage();

    auto &image = mCache[name];
    if (!image.image) {
        image.image = new QImage(image.absolutFileName);
        if (image.image->isNull()) {
            delete image.image;
            image.image = nullptr;
        }
    }

    return image.image ? *image.image : (*this)["invalid.jpg"];

}

//-------------------------------------------------------------------------------------------------
void BzTextures::reset()
{
    for (auto entry: mCache.values()) {
        if (entry.image)
            delete entry.image;
    }
    mCache.clear();
}

//-------------------------------------------------------------------------------------------------
void BzTextures::addFolder(const QString &folder)
{
    QDir dir(folder);

    QStringList files = dir.entryList(QDir::Files);
    for(auto file: files) {
        mCache[file] = MetaImage(file,dir.absoluteFilePath(file));
    }

}
