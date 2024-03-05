#include <QDebug>

#include <libqtr3d/qtr3dgeometrystate.h>
#include "bzbody.h"

//-------------------------------------------------------------------------------------------------
BzBody::BzBody(Type t, double massInTons)
 : mType(t)
 , mAgeMs(0)
 , mMass(massInTons)
 , mCollisionRadius(0)
 , mRepresentation(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
BzBody::~BzBody()
{
    if (mRepresentation)
        mRepresentation->deleteLater();
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
    mAgeMs += ms;
    mGlobalPos += mVelocity * ms;

    if (!mSpin.isNull())
        mRotation += mSpin * ms;

    if (mRepresentation) {
        mRepresentation->setPos(mGlobalPos.toFloat());
        mRepresentation->setRotation(mRotation.toFloat());
    }
}

//-------------------------------------------------------------------------------------------------
void BzBody::setRepresentation(Qtr3dGeometryState *geometry)
{
    if (mRepresentation)
        mRepresentation->deleteLater();
    mRepresentation = geometry;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryState *BzBody::representation()
{
    return mRepresentation;
}
