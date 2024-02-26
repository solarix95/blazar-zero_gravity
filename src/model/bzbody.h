#ifndef BZBODY_H
#define BZBODY_H

#include <QList>
#include "bztypes.h"
#include "bzobject.h"

class BzBody : public BzObject
{
    Q_OBJECT
public:
    BzBody(double massInTons = 0.0);
    virtual ~BzBody() = default;

    inline void   setMass(double m) { mMass = m; }
    inline double mass() const      { return mMass; }

    inline void         setGlobalPos(const BzPos &pos) { mGlobalPos = pos; }
    inline const BzPos &globalPos() const              { return mGlobalPos;}

    void setSpeedKmh(const BzVelocity &newSpeedKmH);
    void setSpeedKmS(const BzVelocity &newSpeedKmS);
    void accelerate(const BzForceList &forces, int ms);
    void process(int ms);

protected:

private:
    qint64      mTime;
    double      mMass;
    BzPos       mGlobalPos;
    BzVelocity  mVelocity;
};

using BzBodyList = QList<BzBody*>;

#endif // BZBODY_H
