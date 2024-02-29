#include <QList>
#include <QDebug>

#include "assets/bzconfig.h"
#include "bzmodel.h"
#include "bztypes.h"
#include "bzbody.h"
#include "bzplanet.h"

//-------------------------------------------------------------------------------------------------
BzModel::BzModel()
{
}

//-------------------------------------------------------------------------------------------------
BzModel::~BzModel()
{
    reset();
}

//-------------------------------------------------------------------------------------------------
void BzModel::deserialize(const BzConfig &cfg)
{
    if (!cfg.isValid())
        return;

    reset();

    auto planets  = cfg.childsByType("planet");

    for (const auto &planetCfg: planets) {
        double radius   = planetCfg.parameter("radius").toDouble();
        if (radius <= 0)
            continue;

        double mass     = planetCfg.parameter("mass").toDouble();
        if (mass <= 0)
            continue;

        QString texture = planetCfg.parameter("texture").toString();
        if (texture.isEmpty())
            continue;

        auto *planet   = new BzPlanet(mass,radius,texture);

        planet->setIdent(planetCfg.parameter("name").toString());

        BzVector3D vector;
        if (planetCfg.parameter("position",vector))
            planet->setGlobalPos(vector);
        if (planetCfg.parameter("spin",vector))
                planet->setSpin(vector);
        if (planetCfg.parameter("velocity",vector))
                planet->setVelocity(vector);
        addBody(planet);
    }

    mWorldRadius = cfg.parameter("worldradius",10000).toDouble();
    mWorldTexture= cfg.parameter("worldtexture").toString();

    emit loaded();
}

//-------------------------------------------------------------------------------------------------
void BzModel::reset()
{
    emit aboutToReset();
    qDeleteAll(mBodies);
    mBodies.clear();
    mWorldTexture.clear();
    mWorldRadius = 0;
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
