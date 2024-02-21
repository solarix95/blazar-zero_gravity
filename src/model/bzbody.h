#ifndef BZBODY_H
#define BZBODY_H

#include "bztypes.h"

class BzBody
{
public:
    BzBody(double massInTons = 0.0);

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
    double      mMass;
    BzPos       mGlobalPos;
    BzVelocity  mVelocity;
};

#endif // BZBODY_H
