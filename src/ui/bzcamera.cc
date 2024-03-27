#include "bzcamera.h"

#include <libqtr3d/qtr3dgeometrystate.h>
#include <libqtr3d/qtr3dcamera.h>

#include "types/bzunits.h"
#include "model/bzbody.h"

//-------------------------------------------------------------------------------------------------
BzCamera::BzCamera()
    : QObject()
    , mCamera(nullptr)
    , mBody(nullptr)
    , mPosController(BzVector3D(0.01,0.01,0.01),BzVector3D(0.0001,0.0001,0.001),BzVector3D(0.1,0.1,0.1))
    , mDirController(BzVector3D(0.1,0.1,0.1),BzVector3D(0.00001,0.00001,0.00001),BzVector3D(1,1,1))
    , mUpController(BzVector3D(0.1,0.1,0.1),BzVector3D(0.00001,0.00001,0.00001),BzVector3D(1,1,1))
{
    mMode = FirstMode;
}

//-------------------------------------------------------------------------------------------------
BzCamera::~BzCamera()
{
}

//-------------------------------------------------------------------------------------------------
void BzCamera::nextMode()
{
    if (mMode == LastMode)
        mMode = FirstMode;
    mMode = (Mode)(mMode+1);
    init();
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

    auto target  = targetPosition();
    auto current = mCamera->pos();

    auto diff    = mPosController.calculate(target,current,ms/20.0);

    // mCamera->lookAt((current+diff).toFloat(),mBody->globalPos().toFloat(),{0,1,0});
    // mCamera->lookAt((mBody->globalPos() + Qtr3dDblVector3D{3*mBody->collisionRadius(),0.0,0.0}),mBody->globalPos(),{0,1,0});
    // mCamera->lookAt(target,{0,0,0},{0,1,0});
    // mCamera->lookAt(targetPosition(),mBody->globalPos(),{0,1,0});

    auto dir = mBody->globalPos().normalized();

    if (dir.isNull())
        dir = Qtr3dDblVector3D(1.0,0.0,1.0).normalized();

    auto pos = mBody->globalPos() + mZoom*dir*3.0*mBody->collisionRadius();

    // qDebug() << mBody->globalPos().toFloat() << dir.toFloat() << mBody->collisionRadius() << pos.toFloat();

     // qDebug() << BzUnit::meters2ogl(mBody->globalPos()).toFloat() << mBody->representation()->pos().toFloat();
     // qDebug() << BzUnit::meters2ogl(mBody->collisionRadius())     << mBody->representation()->radius();

     auto pos1 = mBody->representation()->pos()        + mBody->representation()->pos().normalized()*3.0*mBody->representation()->radius();
     auto pos2 = BzUnit::meters2ogl(mBody->globalPos() + mBody->globalPos().normalized()*mZoom*3.0*mBody->collisionRadius());
     // qDebug() << pos1.toFloat() << pos2.toFloat();

    // mCamera->lookAt(BzUnit::meters2ogl(pos),-dir /*BzUnit::meters2ogl(mBody->globalPos())*/,{0,1,0});
    mCamera->lookAt(BzUnit::meters2ogl(pos),{0,0,0} /*BzUnit::meters2ogl(mBody->globalPos())*/,{0,1,0});


     // mCamera->lookAt(mBody->representation()->pos() + mBody->representation()->pos().normalized()*3.0*mBody->representation()->radius(),-dir /*BzUnit::meters2ogl(mBody->globalPos())*/,{0,1,0});
     //mCamera->lookAt(pos2,-dir /*BzUnit::meters2ogl(mBody->globalPos())*/,{0,1,0});


     // mCamera->lookAt({3*63710,3*63710,3*63710},{0,0,0},{0,1,0});
    // mCamera->lookAt({3*63710,3*63710,3*63710},{0,0,0},{0,1,0});

    // qDebug() << mBody->collisionRadius() << targetPosition().toFloat() << mBody->globalPos().toFloat() << mBody->representation()->pos().toFloat() << mCamera->pos().toFloat();
    // qDebug() << BzUnit::meters2ogl(pos).toFloat() << BzUnit::meters2ogl(mBody->globalPos()).toFloat();
}

//-------------------------------------------------------------------------------------------------
void BzCamera::zoom(float delta)
{
    mZoom += (delta/120.0)/10.0;
}

//-------------------------------------------------------------------------------------------------
void BzCamera::init()
{
    if (!mCamera || !mBody)
        return;

    mZoom = 1;
    onModeChange();
}

//-------------------------------------------------------------------------------------------------
BzVector3D BzCamera::targetPosition() const
{
    Q_ASSERT(mBody);

    BzVector3D dir  = targetDir();
    double distance = 3*mBody->collisionRadius();

    return mBody->globalPos() + dir *distance;
}

//-------------------------------------------------------------------------------------------------
BzVector3D BzCamera::targetDir() const
{
    Q_ASSERT(mBody);
    BzVector3D dir;

    switch(mMode) {
    case OrientationMode:
        dir =  mBody->orientation();
        break;
    case OrbitalMode:
        if (mBody->parentBody())
            dir = (mBody->globalPos() - mBody->parentBody()->globalPos()).normalized();
    }

    if (dir.isNull())
        dir = BzVector3D(0,0,1);

    return dir;
}

//-------------------------------------------------------------------------------------------------
BzVector3D BzCamera::targetUp() const
{
    return BzVector3D(0,1,0);
}

//-------------------------------------------------------------------------------------------------
void BzCamera::onModeChange()
{
    auto target  = targetPosition();
    // Q_ASSERT(0);
    // mCamera->lookAt(target,mBody->globalPos().toFloat(),{0,1,0});
    mPosController.reset();
}
