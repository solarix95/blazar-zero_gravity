#include <QDebug>
#include "bzbody.h"

//-------------------------------------------------------------------------------------------------
BzBody::BzBody(double massInTons)
 : mMass(massInTons)
 , mTime(0)
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
    mVelocity = newSpeedKmS;              // Blazar uses m/ms
}

//-------------------------------------------------------------------------------------------------
void BzBody::accelerate(const BzForceList &forces, int ms)
{
    for (const auto &force: forces)
        mVelocity += force/mMass * ms;
}

//-------------------------------------------------------------------------------------------------
void BzBody::process(int ms)
{
    mTime += ms;
    mGlobalPos += mVelocity * ms;

    /*
    if (ident() == "earth")
        qDebug() << ident() << mTime << 100*(mTime/(1000.0*3600*24*365)) << "[%]" << mTime/(1000.0*3600)<< "[h]"
                 << mVelocity
                 << mGlobalPos;
    */
}
