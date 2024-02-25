#ifndef BZTEXTURES_H
#define BZTEXTURES_H

#include <QObject>
#include <QImage>
#include <QMap>
#include <QString>

class BzTextures
{
public:
    BzTextures();
    virtual ~BzTextures();

    QImage operator[](const char *name) const;
    QImage operator[](const QString &name) const;

    void reset();
    void addFolder(const QString &folder);

private:
    struct MetaImage {
        QString fileName;
        QString absolutFileName;
        QImage  *image;
        MetaImage(QString f="", QString af="", QImage *i=nullptr)
            : fileName(f), absolutFileName(af), image(i) {}
    };

    mutable QMap<QString,MetaImage> mCache;
};

#endif // BZTEXTURES_H
