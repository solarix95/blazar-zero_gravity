#ifndef BZBODY_H
#define BZBODY_H

#include <QList>
#include "bztypes.h"
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

    inline Type   type() const      { return mType;   }

    inline void   setMass(double m) { mMass = m;      }
    inline double mass() const      { return mMass;   }

    inline void    setParentBodyName(const QString &name) { mParentBodyName = name;   }
    inline QString parentBodyName() const                 { return mParentBodyName;   }
    inline void    setParentBody(BzBody *parent)          { mParentBody = parent;     }
    inline BzBody *parentBody() const                     { return mParentBody;       }
    inline void    setRelativePos(const BzPos &pos)       { mRelativePos = pos;       }


    inline void   setCollisionRadius(double r) { mCollisionRadius = r;      }
    inline double collisionRadius() const      { return mCollisionRadius;   }

    inline void         setGlobalPos(const BzPos &pos)       { mGlobalPos = pos; }
    inline const BzPos &globalPos() const                    { return mGlobalPos;}
    inline void         setSpin(const BzPos &spin)           { mSpin = spin;     }
    inline void         setVelocity(const BzVelocity &v)     { mVelocity = v;    }

    void setSpeedKmh(const BzVelocity &newSpeedKmH);
    void setSpeedKmS(const BzVelocity &newSpeedKmS);
    void accelerate(const BzForceList &forces, float ms);
    void process(float ms);
    void finalizeSetup();

    void setRepresentation(Qtr3dGeometryState *geometry);
    Qtr3dGeometryState *representation();

protected:

private:
    BzPos globalPosByParent() const;

    Type                mType;
    qint64              mAgeMs;
    double              mMass;
    double              mCollisionRadius;
    BzPos               mGlobalPos;
    BzRotation          mRotation;
    BzVelocity          mVelocity;
    BzSpin              mSpin;

    // Parent/Child
    QString             mParentBodyName; // to initialze "mParentBody"
    BzPos               mRelativePos;     // to initialze "mGlobalPos"
    BzBody             *mParentBody;

    Qtr3dGeometryState *mRepresentation;
};

using BzBodyList = QList<BzBody*>;

#endif // BZBODY_H
