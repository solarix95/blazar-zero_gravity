#ifndef BZASSETS_H
#define BZASSETS_H

#include <QObject>

class BzTextures;
class BzConfigs;

class BzAssets
{
public:
    BzAssets();

    void init(const QString &basedir);

    const BzTextures &textures()  const;
    const BzConfigs  &scenarios() const;

private:
    BzTextures *mTextures;
    BzConfigs  *mScenarios;
};

#endif // BZASSETS_H
