#ifndef BZCONFIGS_H
#define BZCONFIGS_H

#include <QString>
#include <QStringList>
#include "bzconfig.h"

class BzConfigs
{
public:
    BzConfigs();

    void        addFolder(const QString &folder);

    BzConfig    byName(const QString &name) const;
    QStringList names() const;

private:
    QList<BzConfig> mConfigs;
};

#endif // BZCONFIGS_H
