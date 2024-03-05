#include "bzplanet.h"

BzPlanet::BzPlanet(double mass, double radius, const QString &textureName)
 : BzBody(CelestialType,mass)
 , mRadius(radius)
 , mTextureName(textureName)
{
    setCollisionRadius(radius);
}
