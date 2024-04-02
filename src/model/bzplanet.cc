#include "assets/bzconfig.h"
#include "bzplanet.h"

//-------------------------------------------------------------------------------------------------
BzPlanet::BzPlanet(double mass, double radius, const QString &textureName)
 : BzBody(CelestialType,mass)
 , mRadius(radius)
 , mTextureName(textureName)
{
    setCollisionRadius(radius);
}

//-------------------------------------------------------------------------------------------------
bool BzPlanet::deserialize(const BzConfig &config)
{
    if (!BzBody::deserialize(config))
        return false;

    mRadius = config.parameter("radius").toDouble();
    if (mRadius <= 0)
        return false;
    setCollisionRadius(mRadius);

    mTextureName = config.parameter("texture").toString();
    if (mTextureName.isEmpty())
        return false;;

    return true;
}
