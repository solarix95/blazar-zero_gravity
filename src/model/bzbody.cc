#include "bzbody.h"

//-------------------------------------------------------------------------------------------------
BzBody::BzBody()
{
}

//-------------------------------------------------------------------------------------------------
void BzBody::setSpeedKmh(const BzVelocity &newSpeedKmH)
{
    mVelocity = newSpeedKmH * 0.000277778; // Blazar uses m/ms
}

//-------------------------------------------------------------------------------------------------
void BzBody::setSpeedKmS(const BzVelocity &newSpeedKmS)
{
    mVelocity = newSpeedKmS * 1000;       // Blazar uses m/ms
}

//-------------------------------------------------------------------------------------------------
void BzBody::accelerate(const BzForceList &forces, int ms)
{
    for (const auto &force: forces)
        mVelocity += force * ms;
}

//-------------------------------------------------------------------------------------------------
void BzBody::process(int ms)
{
    mGlobalPos += mVelocity * ms;
}
