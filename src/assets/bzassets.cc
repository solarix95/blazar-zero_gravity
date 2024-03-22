#include <QDir>

#include "bzassets.h"
#include "bztextures.h"
#include "bzconfigs.h"
#include "bzfilebuffer.h"

//-------------------------------------------------------------------------------------------------
BzAssets::BzAssets()
 : mTextures(nullptr)
 , mScenarios(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void BzAssets::init(const QString &basedir, const QString &userdir)
{
    Q_ASSERT(!mTextures);
    mTextures = new BzTextures();
    mTextures->addFolder(basedir + "/textures");

    mScenarios = new BzConfigs();
    mScenarios->addFolder(basedir + "/scenarios");

    mModels = new BzFileBuffer();
    mModels->addFolder(basedir + "/models");

    if (!userdir.isEmpty()) {
        mTextures->addFolder(userdir + "/textures");
        mScenarios->addFolder(userdir + "/scenarios");
        mModels->addFolder(userdir + "/models");
    }
}

//-------------------------------------------------------------------------------------------------
const BzTextures &BzAssets::textures() const
{
    Q_ASSERT(mTextures);
    return *mTextures;
}

//-------------------------------------------------------------------------------------------------
const BzConfigs &BzAssets::scenarios() const
{
    Q_ASSERT(mScenarios);
    return *mScenarios;
}

//-------------------------------------------------------------------------------------------------
const BzFileBuffer &BzAssets::models() const
{
    Q_ASSERT(mModels);
    return *mModels;
}
