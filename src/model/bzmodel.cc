#include <QList>
#include <QDebug>

#include "assets/bzconfig.h"
#include "bzmodel.h"
#include "bztypes.h"
#include "bzbody.h"
#include "bzpart.h"
#include "bzplanet.h"

//-------------------------------------------------------------------------------------------------
BzModel::BzModel()
 : mActiveBody(nullptr)
 , mTimeScale(1)
 , mTargetTimeScale(1)
 , mCurrentTimeScale(mTargetTimeScale)
 , mDeltaTimeScale(0)
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

    deserializePlanets(cfg.childsByType("planet"));
    deserializeBodies(cfg.childsByType("part"));

    mWorldRadius = cfg.parameter("worldradius",10000).toDouble();
    mWorldTexture= cfg.parameter("worldtexture").toString();

    setTimeScale(1.0);
    mCurrentTimeScale = mTargetTimeScale = 1;
    mDeltaTimeScale   = 0.0;

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
    if (body->type() == BzBody::CelestialType)
        mCelestials << body;
}

//-------------------------------------------------------------------------------------------------
void BzModel::setTimeScale(int newScale)
{
    if (mTimeScale == newScale)
        return;
    mTimeScale = newScale;
    emit timeScaleChanged(mTimeScale);

    if (mTimeScale < 0) {
        mTargetTimeScale  = 0;
        mCurrentTimeScale = 0;
        mDeltaTimeScale   = 0;
        return;
    }
    if (mTimeScale == 0)
        mTargetTimeScale = 0.5;
    if (mTimeScale > 0)
        mTargetTimeScale = mTimeScale;

    mDeltaTimeScale = (mTargetTimeScale - mCurrentTimeScale)/1000;
}

//-------------------------------------------------------------------------------------------------
void BzModel::incTimeScale(int diff)
{
    if (diff > 0) {
        if (mTimeScale < 4)
            setTimeScale(mTimeScale+1);
        else {
            if (mTimeScale < 10)   // 1..2..3..4....10..100..1000
                setTimeScale(10);
            else
                setTimeScale(mTimeScale*10);
        }
    }
    if (diff < 0) {
        if (mTimeScale <= -1)
            return;
        if (mTimeScale <= 4) {
            setTimeScale(mTimeScale-1);
            return;
        }
        if (mTimeScale == 10) {
            setTimeScale(4);
            return;
        }

        Q_ASSERT((mTimeScale % 10) == 0);
        setTimeScale(mTimeScale/10);
    }
}

//-------------------------------------------------------------------------------------------------
void BzModel::activateNextBody()
{
    if (mBodies.isEmpty())
        return;
    if (!mActiveBody) {
        mActiveBody = mBodies.first();
    } else {
        int index = mBodies.indexOf(mActiveBody);
        Q_ASSERT(index >= 0);
        index++;
        if (index >= mBodies.count())
            index = 0;
        mActiveBody = mBodies[index];
    }
    emit bodyActivated(mActiveBody);
}


//-------------------------------------------------------------------------------------------------
void BzModel::process(int ms)
{
    processLeapfrog(ms);

    if (std::abs(mCurrentTimeScale - mTargetTimeScale) > 0.0001f) {
        mCurrentTimeScale += mDeltaTimeScale * ms;
        mDeltaTimeScale = (mTargetTimeScale - mCurrentTimeScale)/1000;
        emit timeScaleChanged(mCurrentTimeScale);
        qDebug() << mTimeScale << mCurrentTimeScale;
    }
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
        for(const auto &other: mCelestials) {
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

//-------------------------------------------------------------------------------------------------
void BzModel::deserializePlanets(const QList<BzConfig> &planetConfig)
{
    for (const auto &planetCfg: planetConfig) {
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
}

//-------------------------------------------------------------------------------------------------
void BzModel::deserializeBodies(const QList<BzConfig> &bodiesConfig)
{
    for (const auto &planetCfg: bodiesConfig) {

        double mass     = planetCfg.parameter("mass").toDouble();
        if (mass <= 0)
            continue;

        QString modelname = planetCfg.parameter("model").toString();
        if (modelname.isEmpty())
            continue;

        auto *part   = new BzPart(mass);

        part->setIdent(planetCfg.parameter("name").toString());
        part->displayInfo().modelName = modelname;

        BzVector3D vector;
        if (planetCfg.parameter("position",vector))
            part->setGlobalPos(vector);
        if (planetCfg.parameter("spin",vector))
            part->setSpin(vector);
        if (planetCfg.parameter("velocity",vector))
            part->setVelocity(vector);
        addBody(part);
    }
}
