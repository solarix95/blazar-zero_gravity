#ifndef BZFILEBUFFER_H
#define BZFILEBUFFER_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QMap>

class BzFileBuffer
{
public:
    BzFileBuffer();
    virtual ~BzFileBuffer();

    QByteArray operator[](const char *name) const;
    QByteArray operator[](const QString &name) const;
    QString    absolutFileName(const QString &name) const;

    void reset();
    void addFolder(const QString &folder);

private:
    struct MetaFile {
        QString fileName;
        QString absolutFileName;
        QByteArray  *buffer;
        MetaFile(QString f="", QString af="", QByteArray *i=nullptr)
            : fileName(f), absolutFileName(af), buffer(i) {}
    };

    mutable QMap<QString,MetaFile> mCache;
};

#endif // BZFILEBUFFER_H
