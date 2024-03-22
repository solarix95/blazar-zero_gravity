#ifndef BZCONFIG_H
#define BZCONFIG_H

#include <QList>
#include <QVariant>
#include "types/bztypes.h"

/*
    Config (De-)Serialization

    Simplyfied JSON:
    ----------------------------------------------------------------------
    {
        # Parameter of object 1
        key1 = value
        key2 = value

        # Start of child object 2
        {
            # Parameter of object 2
            key1 = value
            key2 = value
        }
    }
*/

class BzConfig
{
public:
    BzConfig();

    bool fromFile(const QString &fileName);
    bool fromData(const QByteArray &data);
    bool fromData(const QByteArray &data, int &pos);

    bool           isValid() const;
    QVariant       parameter(const QString &name, const QVariant &defaultValue = QVariant()) const;
    bool           parameter(const QString &name, BzVector3D &vector) const;

    QList<BzConfig> childsByType(const QString &typeName) const;
    QList<BzConfig> childsByAttribut(const QString &attributName, const QVariant &attributValue) const;

private:
    static bool skipEmpty(const QByteArray &data, int &pos);
    static bool getKeyValue(const QByteArray &data, int &pos, QString &key, QString &value);

    QList<BzConfig>        mChilds;
    QMap<QString,QVariant> mParameters;
};

#endif // BZCONFIG_H
