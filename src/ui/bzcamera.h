#ifndef BZCAMERA_H
#define BZCAMERA_H

#include <QObject>
#include <QVector3D>

#include "model/bztypes.h"
#include <libqtr3d/utils/qtr3dpidcontroller.h>
class Qtr3dCamera;
class BzBody;
class BzCamera : public QObject
{
public:
    enum Mode {
        FreeMode,     // Camera independent from flight vector
        FollowMode,   // Follows flight vector, with PID controller
        LockedMode,   // Follows flight vector, but static relative position/orientation
        OrbitalMode   // Follows flight vector, Planet is always "Bottom"
    };
    BzCamera();
    virtual ~BzCamera();


    void setCamera(Qtr3dCamera *camera);
    void follow(BzBody *body);
    void process(int ms);

private:
    void init();

    Mode         mMode;
    Qtr3dCamera *mCamera;
    BzBody      *mBody;

    Qtr3dPidController<BzVector3D> mPosController;
    Qtr3dPidController<BzVector3D> mDirController;
    Qtr3dPidController<BzVector3D> mUpController;
};

#endif // BZCAMERA_H
