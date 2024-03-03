#include <QFile>
#include <QDir>
#include "bzfilebuffer.h"

//-------------------------------------------------------------------------------------------------
BzFileBuffer::BzFileBuffer()
{
}

//-------------------------------------------------------------------------------------------------
BzFileBuffer::~BzFileBuffer()
{
   reset();
}

//-------------------------------------------------------------------------------------------------
QByteArray BzFileBuffer::operator [](const char *name) const
{
    return (*this)[QString::fromUtf8(name)];
}

//-------------------------------------------------------------------------------------------------
QByteArray BzFileBuffer::operator[](const QString &name) const
{
    if (!mCache.contains(name))
        return QByteArray();

    auto &meta = mCache[name];
    if (!meta.buffer) {
        meta.buffer = new QByteArray();
        QFile f(meta.absolutFileName);
        if (f.open(QIODevice::ReadOnly))
            (*meta.buffer) = f.readAll();
    }

    return meta.buffer ? *meta.buffer : QByteArray();
}

//-------------------------------------------------------------------------------------------------
void BzFileBuffer::reset()
{
    for (auto entry: mCache.values()) {
        if (entry.buffer)
            delete entry.buffer;
    }
    mCache.clear();
}

//-------------------------------------------------------------------------------------------------
void BzFileBuffer::addFolder(const QString &folder)
{
    QDir dir(folder);

    QStringList files = dir.entryList(QDir::Files);
    for(auto file: files) {
        mCache[file] = MetaFile(file,dir.absoluteFilePath(file));
    }
}
