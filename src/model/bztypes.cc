#include <math.h>
#include "bztypes.h"

#define EPSILON 1e-8

//-------------------------------------------------------------------------------------------------
bool BzVector3D::isNull() const
{
    return (x*x + y*y + z*z) < EPSILON * EPSILON;
}

//-------------------------------------------------------------------------------------------------
double BzVector3D::distanceToPoint(const BzVector3D &other) const
{
    return std::sqrt((x - other.x) * (x - other.x) +
                     (y - other.y) * (y - other.y) +
                     (z - other.z) * (z - other.z));
}

//-------------------------------------------------------------------------------------------------
double BzVector3D::length() const
{
    return std::sqrt(x*x + y*y + z*z);
}

//-------------------------------------------------------------------------------------------------
BzVector3D BzVector3D::normalized() const
{
    double magnitude = sqrt(x * x + y * y + z * z);
    if (magnitude < EPSILON)
        return BzVector3D(0, 0, 0); // Return a zero vector if the original vector is too small
    return BzVector3D(x / magnitude, y / magnitude, z / magnitude);
}

//-------------------------------------------------------------------------------------------------
void BzVector3D::normalize()
{
    double magnitude = sqrt(x * x + y * y + z * z);
    if (magnitude < EPSILON) {
        x = y = z = 0.0;
        return;
    }

    x = x / magnitude;
    y = y / magnitude;
    z = z / magnitude;
    if (x < EPSILON)
        x = 0.0;
    if (y < EPSILON)
        y = 0.0;
    if (z < EPSILON)
        z = 0.0;
}

//-------------------------------------------------------------------------------------------------
BzVector3D &BzVector3D::operator+=(const BzVector3D &vector)
{
    x += vector.x;
    y += vector.y;
    z += vector.z;
    return *this;
}

//-------------------------------------------------------------------------------------------------
const BzVector3D operator+(const BzVector3D &v1, const BzVector3D &v2)
{
    return BzVector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

//-------------------------------------------------------------------------------------------------
const BzVector3D operator-(const BzVector3D &v1, const BzVector3D &v2)
{
    return BzVector3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

//-------------------------------------------------------------------------------------------------
const BzVector3D operator*(double factor, const BzVector3D &vector)
{
    return BzVector3D(vector.x*factor, vector.y*factor, vector.z*factor);
}

//-------------------------------------------------------------------------------------------------
const BzVector3D operator/(const BzVector3D &vector, double divisor)
{
    Q_ASSERT(divisor != 0);
    return BzVector3D(vector.x/divisor, vector.y/divisor, vector.z/divisor);
}

//-------------------------------------------------------------------------------------------------
const BzVector3D operator*(const BzVector3D &vector, double factor)
{
    return BzVector3D(vector.x*factor, vector.y*factor, vector.z*factor);
}

//-------------------------------------------------------------------------------------------------
bool operator==(const BzVector3D &v1, const BzVector3D &v2)
{
    return std::fabs(v1.x - v2.x) < EPSILON &&
           std::fabs(v1.y - v2.y) < EPSILON &&
           std::fabs(v1.z - v2.z) < EPSILON;
}

//-------------------------------------------------------------------------------------------------
bool operator!=(const BzVector3D &v1, const BzVector3D &v2)
{
    return !(v1 == v2);
}
