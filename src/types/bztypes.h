#ifndef BZTYPES_H
#define BZTYPES_H

#include <QList>
#include "libqtr3d/math3d/qtr3ddblvector.h"

using BzVector3D = Qtr3dDblVector3D;
using BzPos      = Qtr3dDblVector3D;
using BzPosList  = QList<BzPos>;

using BzVelocity      = Qtr3dDblVector3D;
using BzVelocityList  = QList<BzVelocity>;

using BzForce         = Qtr3dDblVector3D;
using BzForceList     = QList<BzForce>;

using BzSpin          = Qtr3dDblVector3D;
using BzRotation      = Qtr3dDblVector3D;
#endif // BZTYPES_H
