#include "bzpart.h"

//-------------------------------------------------------------------------------------------------
BzPart::BzPart(double massInTons)
 : BzBody(PartType, massInTons)
{

}

//-------------------------------------------------------------------------------------------------
bool BzPart::deserialize(const BzConfig &config)
{
    if (!BzBody::deserialize(config))
        return false;

    mDisplayInfo.modelName = config.parameter("model").toString();
    if (mDisplayInfo.modelName.isEmpty())
        return false;

    auto radius = config.parameter("radius").toDouble();
    if (radius <= 0)
        return false;
    setCollisionRadius(radius);

    return true;
}
