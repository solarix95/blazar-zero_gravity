#include <math.h>
#include "bzunits.h"

#define OGL_SCALE 100.0

double BzUnit::meters2ogl(double distanceMeters)
{
    return distanceMeters / OGL_SCALE;
}

BzVector3D BzUnit::meters2ogl(const BzVector3D &distanceMeters)
{
    return distanceMeters/OGL_SCALE;
}
