#ifndef BZASSETS_H
#define BZASSETS_H

#include <QObject>

class BzTextures;
class BzConfigs;
class BzFileBuffer;

class BzAssets
{
public:
    BzAssets();

    void init(const QString &basedir);

    const BzTextures   &textures()  const;
    const BzConfigs    &scenarios() const;
    const BzFileBuffer &models()    const;

private:
    BzTextures   *mTextures;
    BzConfigs    *mScenarios;
    BzFileBuffer *mModels;
};

#endif // BZASSETS_H
