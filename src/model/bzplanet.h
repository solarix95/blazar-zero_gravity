#ifndef BZPLANET_H
#define BZPLANET_H

#include "bzbody.h"
#include <QString>

class BzPlanet : public BzBody
{
public:
    BzPlanet(double mass, double radius, const QString &textureName);

    inline double  radius() const       { return mRadius; }
    inline QString textureName() const  { return mTextureName; }

private:
    double  mRadius;
    QString mTextureName;
};

#endif // BZPLANET_H
