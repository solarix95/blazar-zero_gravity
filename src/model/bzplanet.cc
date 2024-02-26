#include "bzplanet.h"

BzPlanet::BzPlanet(double mass, double radius, const QString &textureName)
 : BzBody(mass)
 , mRadius(radius)
 , mTextureName(textureName)
{
}
