#include "bzcamera.h"

#include <libqtr3d/qtr3dcamera.h>
#include "model/bzbody.h"

//-------------------------------------------------------------------------------------------------
BzCamera::BzCamera()
 : QObject()
 , mCamera(nullptr)
 , mBody(nullptr)
 , mPosController(BzVector3D(0.1,0.1,0.1),BzVector3D(0.00001,0.00001,0.0001),BzVector3D(1,1,1))
 , mDirController(BzVector3D(0.1,0.1,0.1),BzVector3D(0.00001,0.00001,0.00001),BzVector3D(1,1,1))
 , mUpController(BzVector3D(0.1,0.1,0.1),BzVector3D(0.00001,0.00001,0.00001),BzVector3D(1,1,1))
{
}

//-------------------------------------------------------------------------------------------------
BzCamera::~BzCamera()
{
}

//-------------------------------------------------------------------------------------------------
void BzCamera::setCamera(Qtr3dCamera *camera)
{
    mCamera = camera;
    init();
}

//-------------------------------------------------------------------------------------------------
void BzCamera::follow(BzBody *body)
{
    if (mBody == body)
        return;

    if (mBody)
        mBody->disconnect(this);

    mBody = body;
    if (!mBody)
        return;

    connect(mBody,&QObject::destroyed, this, [this,body]() {
        if (body == mBody)
            mBody = nullptr;
    });

    init();
}

//-------------------------------------------------------------------------------------------------
void BzCamera::process(int ms)
{
    if (!mCamera || !mBody)
        return;

    auto target  = (mBody->globalPos() + mBody->globalPos().normalized() * mBody->collisionRadius() * 3);
    auto current = mCamera->pos();

    auto diff    = mPosController.calculate(target,current,1);

    mCamera->lookAt((current-diff).toFloat(),mBody->globalPos().toFloat(),{0,1,0});
}

//-------------------------------------------------------------------------------------------------
void BzCamera::init()
{
    if (!mCamera || !mBody)
        return;

    auto target  = (mBody->globalPos() + mBody->globalPos().normalized() * mBody->collisionRadius() * 3).toFloat();
    mCamera->lookAt(target,mBody->globalPos().toFloat(),{0,1,0});
}
