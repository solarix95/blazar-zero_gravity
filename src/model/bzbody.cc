#include <QDebug>

#include <libqtr3d/qtr3dgeometrystate.h>
#include "bzbody.h"
#include "types/bzunits.h"

//-------------------------------------------------------------------------------------------------
BzBody::BzBody(Type t, double massInTons)
 : mType(t)
 , mAgeMs(0)
 , mMass(massInTons)
 , mCollisionRadius(0)
 , mParentBody(nullptr)
 , mRepresentation(nullptr)
{
    mOrientation = BzVector3D(0,0,1);
    mUp          = BzVector3D(0,1,0);
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
void BzBody::accelerate(const BzForceList &forces, float ms)
{
    /*
    if (ident() == "Moon") {
        qDebug() << "Moon Acceleration" << mVelocity << forces << ms << (forces.first() * ms/1000.0) << (mVelocity + (forces.first() * ms/1000.0));
        qDebug() << mVelocity.z << ((forces.first() * ms/1000.0)).z << (mVelocity.z + ((forces.first() * ms/1000.0)).z);
    }
    */
    for (const auto &force: forces)
        mVelocity += (force * ms/1000.0);
}

//-------------------------------------------------------------------------------------------------
void BzBody::process(float ms)
{
    mAgeMs += ms;
    mGlobalPos += mVelocity * ms/1000.0;

    if (!mSpin.isNull())
        mRotation += mSpin * ms/1000.0;

    /*
    if (ident() == "Moon")
        qDebug() << "Moon Velocity" << mVelocity;

    */

    if (mRepresentation) {
        mRepresentation->setPos(BzUnit::meters2ogl(mGlobalPos));
        mRepresentation->setRotation(mRotation.toFloat());
    }
}

//-------------------------------------------------------------------------------------------------
void BzBody::finalizeSetup()
{
    setGlobalPos(globalPosByParent());
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

//-------------------------------------------------------------------------------------------------
BzPos BzBody::globalPosByParent() const
{
    if (!mParentBody)
        return globalPos();
    return mParentBody->globalPosByParent() + mRelativePos;
}
