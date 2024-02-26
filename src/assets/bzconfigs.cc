#include <QDir>
#include "bzconfigs.h"

//-------------------------------------------------------------------------------------------------
BzConfigs::BzConfigs()
{
}

//-------------------------------------------------------------------------------------------------
void BzConfigs::addFolder(const QString &folder)
{
    QDir dir(folder);

    QStringList files = dir.entryList(QDir::Files);
    for(auto file: files) {
        if (!file.toLower().endsWith(".cfg"))
            continue;
        BzConfig next;
        if (!next.fromFile(dir.absoluteFilePath(file)))
            continue;
        mConfigs << next;
    }
}

//-------------------------------------------------------------------------------------------------
BzConfig BzConfigs::byName(const QString &name) const
{
    for(const auto &cfg: mConfigs) {
        if (cfg.parameter("name").toString() == name)
            return cfg;
    }
    return BzConfig();
}

//-------------------------------------------------------------------------------------------------
QStringList BzConfigs::names() const
{
    QStringList ret;
    for(const auto &cfg: mConfigs) {
        QString nextName = cfg.parameter("name").toString();
        if (nextName.isEmpty())
            continue;
        ret << nextName;
    }
    return ret;
}
