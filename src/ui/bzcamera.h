#ifndef BZCAMERA_H
#define BZCAMERA_H

#include <QObject>
#include <QVector3D>

#include "types/bztypes.h"
#include <libqtr3d/utils/qtr3dpidcontroller.h>
class Qtr3dCamera;
class BzBody;
class BzCamera : public QObject
{
public:
    enum Mode {
        OrientationMode, // Over-the-Shoulder Camera
        OrbitalMode,     // Follows flight vector, Planet is always "Bottom"

        // Mode Rotation
        FirstMode  = OrientationMode,
        LastMode   = OrbitalMode,
        /*
        FollowMode,      // Follows flight vector, "down" is orientation
        LockedMode,     // Follows flight vector, but static relative position/orientation
        OrbitalMode     // Follows flight vector, Planet is always "Bottom"
        */
    };
    BzCamera();
    virtual ~BzCamera();

    void nextMode();
    void setCamera(Qtr3dCamera *camera);
    void follow(BzBody *body);
    void process(int ms);

private:
    void       init();

    BzVector3D targetPosition() const;
    BzVector3D targetDir() const;
    BzVector3D targetUp() const;

    void       onModeChange();

    Mode         mMode;
    Qtr3dCamera *mCamera;
    BzBody      *mBody;

    Qtr3dPidController<BzVector3D> mPosController;
    Qtr3dPidController<BzVector3D> mDirController;
    Qtr3dPidController<BzVector3D> mUpController;
};

#endif // BZCAMERA_H
