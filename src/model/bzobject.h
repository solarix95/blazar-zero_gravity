#ifndef BZOBJECT_H
#define BZOBJECT_H

#include <QObject>
#include <QString>
#include <assets/bzconfig.h>

class BzObject : public QObject
{
public:
    BzObject();

    virtual bool deserialize(const BzConfig &config);

    inline void        setIdent(const QString &ident) { mIdent = ident; }
    inline QString     ident() const                  { return mIdent;  }
    inline QStringList tags() const                   { return mTags;   }

private:
    QString     mIdent;
    QStringList mTags;
};

#endif // BZOBJECT_H
