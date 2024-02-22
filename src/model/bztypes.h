#ifndef BZTYPES_H
#define BZTYPES_H

#include <QList>
#include <QVector3D>

class BzVector3D {
public:
    double x;
    double y;
    double z;

    BzVector3D(double ax = 0, double ay = 0, double az = 0) : x(ax), y(ay), z(az) {}

    bool       isNull() const;
    double     distanceToPoint(const BzVector3D &other) const;
    double     length() const;
    BzVector3D normalized() const;
    void       normalize();

    BzVector3D &operator+=(const BzVector3D &vector);
    operator QString() const { return QString("X:%1, Y:%2, Z:%3").arg(x,0,'e',24).arg(y).arg(z); }
};

// related non-member operators
const BzVector3D operator+(const BzVector3D &v1, const BzVector3D &v2);
const BzVector3D operator-(const BzVector3D &v1, const BzVector3D &v2);
const BzVector3D operator*(double factor, const BzVector3D &vector);
const BzVector3D operator*(const BzVector3D &vector, double factor);
const BzVector3D operator/(const BzVector3D &vector, double divisor);
bool operator==(const BzVector3D &v1, const BzVector3D &v2);
bool operator!=(const BzVector3D &v1, const BzVector3D &v2);

using BzPos      = BzVector3D;
using BzPosList  = QList<BzPos>;

using BzVelocity      = BzVector3D;
using BzVelocityList  = QList<BzVelocity>;

using BzForce         = BzVector3D;
using BzForceList     = QList<BzForce>;

#endif // BZTYPES_H
