#ifndef BZBODY_H
#define BZBODY_H

#include <QList>
#include <QQuaternion>
#include "types/bztypes.h"
#include "bzobject.h"


class Qtr3dGeometryState;
class BzBody : public BzObject
{
    Q_OBJECT
public:
    enum Type {
        CelestialType,
        PartType
    };

    BzBody(Type t, double massInTons = 0.0);
    virtual ~BzBody();

    bool deserialize(const BzConfig &config);

    inline Type   type() const      { return mType;   }

    inline void   setMass(double m) { mMass = m;      }
    inline double mass() const      { return mMass;   }

    inline void    setParentBodyName(const QString &name) { mParentBodyName = name;   }
    inline QString parentBodyName() const                 { return mParentBodyName;   }
    inline void    setParentBody(BzBody *parent)          { mParentBody = parent;     }
    inline BzBody *parentBody() const                     { return mParentBody;       }
    inline void    setRelativePos(const BzPos &pos)       { mRelativePos = pos;       }
    inline void    setRelativeVelocity(const BzVelocity &v) { mRelativeVelocity = v;       }

    inline void   setCollisionRadius(double r) { mCollisionRadius = r;      }
    inline double collisionRadius() const      { return mCollisionRadius;   }

    inline void         setGlobalPos(const BzPos &pos)       { mGlobalPos = pos;  }
    inline const BzPos &globalPos() const                    { return mGlobalPos; }
    inline void         setRotation(const QQuaternion &spin) { mRotation = spin;  }
    inline void         setVelocity(const BzVelocity &v)     { mVelocity = v;     }

    inline BzVector3D   movementDirection() const            { return mVelocity.normalized(); }

    inline const BzVector3D  &orientation() const            { return mOrientation; }
    inline const BzVector3D  &up() const                     { return mUp; }


    void setSpeedKmh(const BzVelocity &newSpeedKmH);
    void setSpeedKmS(const BzVelocity &newSpeedKmS);
    void accelerate(const BzForceList &forces, float ms);
    void process(float ms);
    void finalizeSetup();

    void setRepresentation(Qtr3dGeometryState *geometry);
    Qtr3dGeometryState *representation();

protected:

private:
    BzPos      globalPosByParent() const;
    BzVelocity globalVelocityByParent() const;

    Type                mType;
    qint64              mAgeMs;
    double              mMass;
    double              mCollisionRadius;

    BzPos               mGlobalPos;
    BzVelocity          mVelocity;

    BzVector3D          mOrientation;
    BzVector3D          mUp;
    QQuaternion         mRotation;

    BzSpin              mSpin;

    // Parent/Child
    QString             mParentBodyName;   // to initialze "mParentBody"
    BzPos               mRelativePos;      // to initialze "mGlobalPos"
    BzVelocity          mRelativeVelocity; // to initialze "mVelocity"

    BzBody             *mParentBody;
    Qtr3dGeometryState *mRepresentation;
};

using BzBodyList = QList<BzBody*>;

#endif // BZBODY_H
