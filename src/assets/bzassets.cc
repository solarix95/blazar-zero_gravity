#include <QDir>

#include "bzassets.h"
#include "bztextures.h"

//-------------------------------------------------------------------------------------------------
BzAssets::BzAssets()
 : mTextures(nullptr)
{

}

//-------------------------------------------------------------------------------------------------
void BzAssets::init(const QString &basedir)
{
    Q_ASSERT(!mTextures);
    mTextures = new BzTextures();
    mTextures->addFolder(basedir + "/textures");
}

//-------------------------------------------------------------------------------------------------
const BzTextures &BzAssets::textures() const
{
    Q_ASSERT(mTextures);
    return *mTextures;
}
