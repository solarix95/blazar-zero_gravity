#include <QList>
#include <QDebug>

#include "bzmodel.h"
#include "bztypes.h"
#include "bzbody.h"

//-------------------------------------------------------------------------------------------------
BzModel::BzModel()
{
}

//-------------------------------------------------------------------------------------------------
void BzModel::addBody(BzBody *body)
{
    Q_ASSERT(body);
    mBodies << body;
}

//-------------------------------------------------------------------------------------------------
void BzModel::process(int ms)
{
    processLeapfrog(ms);
}

//-------------------------------------------------------------------------------------------------
void BzModel::processLeapfrog(int ms)
{
    // qDebug() << "processLeapfrog";
    updateBodyVectors(ms);
    updateBodyPositions(ms);
}

//-------------------------------------------------------------------------------------------------
void BzModel::updateBodyVectors(int ms)
{
    // Update Body Forces + Velocity
    BzForceList forces;
    for(auto &nextBody: mBodies) {
        for(const auto &other: mBodies) {
            if (nextBody == other)
                continue;

            BzVector3D v = (other->globalPos() - nextBody->globalPos());

            float     d = v.length();
            if (d <= 0)
                continue;

            v.normalize();

            forces.append( v * (other->mass()/(d*d)));
        }
        if (forces.isEmpty())
            continue;

        nextBody->accelerate(forces,ms);
        forces.clear();
    }
}

//-------------------------------------------------------------------------------------------------
void BzModel::updateBodyPositions(int ms)
{
    for(auto &nextBody: mBodies)
        nextBody->process(ms);
}
