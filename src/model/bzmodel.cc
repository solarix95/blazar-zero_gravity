#include <QList>
#include <QDebug>

#include "assets/bzconfig.h"
#include "types/bztypes.h"

#include "bzmodel.h"
#include "bzbody.h"
#include "bzpart.h"
#include "bzplanet.h"

#define G_CONSTANT 6.674e-11  // Gravitational constant

//-------------------------------------------------------------------------------------------------
BzModel::BzModel()
 : mActiveBody(nullptr)
 , mTimeScale(1)
 , mMissionTime(0)
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
    auto skyBoxes = cfg.childsByType("skybox");
    if (skyBoxes.count() >= 1) {
        mSkyBox.radius  = skyBoxes.first().parameter("radius").toDouble();
        mSkyBox.texture = skyBoxes.first().parameter("texture").toString();
    }

    resolveBodyParents();
    finalizeSetup();

    // setTimeScale(1.0);
    mTimeScale        = 1;
    mCurrentTimeScale = mTargetTimeScale = 1;
    mDeltaTimeScale   = 0.0;
    mMissionTime      = 0;
    emit timeScaleChanged(mCurrentTimeScale);

    activateNextBody();
    emit loaded();
}

//-------------------------------------------------------------------------------------------------
void BzModel::reset()
{
    emit aboutToReset();
    qDeleteAll(mBodies);
    mActiveBody = nullptr;
    mBodies.clear();
    mSkyBox = SkyBox();
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

    if (mTimeScale < 0) {
        mTargetTimeScale  = 0;
        mCurrentTimeScale = 0;
        mDeltaTimeScale   = 0;
        timeScaleChanged(0);
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
void BzModel::process(float ms, float normalizedTime, int realtime)
{
    mMissionTime += ms;
    processLeapfrog(ms);

    if (std::abs(mCurrentTimeScale - mTargetTimeScale) > 0.0001f) {
        if (realtime > 0)
            mCurrentTimeScale += mDeltaTimeScale * realtime;
        else
            mCurrentTimeScale += mDeltaTimeScale;
        mDeltaTimeScale = (mTargetTimeScale - mCurrentTimeScale)/1000;
        emit timeScaleChanged(mCurrentTimeScale);
        // qDebug() << mTimeScale << mCurrentTimeScale << mDeltaTimeScale;
    }
}

//-------------------------------------------------------------------------------------------------
void BzModel::processLeapfrog(float ms)
{
    // qDebug() << "processLeapfrog";
    updateBodyVectors(ms);
    updateBodyPositions(ms);
}

//-------------------------------------------------------------------------------------------------
void BzModel::updateBodyVectors(float ms)
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

            forces.append( v * 1000.0 /* Tons -> kg */ *((G_CONSTANT*other->mass()/(d*d))));
        }
        if (forces.isEmpty())
            continue;

        nextBody->accelerate(forces,ms);
        forces.clear();
    }
}

//-------------------------------------------------------------------------------------------------
void BzModel::updateBodyPositions(float ms)
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
        planet->setParentBodyName(planetCfg.parameter("parent").toString());

        BzVector3D vector;
        if (planetCfg.parameter("position",vector))
            planet->setGlobalPos(vector);
        if (planetCfg.parameter("relativeposition",vector))
            planet->setRelativePos(vector);
        if (planetCfg.parameter("spin",vector))
            planet->setSpin(vector);
        if (planetCfg.parameter("velocity",vector))
            planet->setVelocity(vector);
        addBody(planet);

        qDebug().noquote() << QString("%1: acceleration on surface: %2[m/s2]").arg(planet->ident()).arg(G_CONSTANT*mass/(radius * radius));
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
        part->setCollisionRadius(planetCfg.parameter("radius",QVariant(0)).toDouble());

        addBody(part);
    }
}

//-------------------------------------------------------------------------------------------------
void BzModel::resolveBodyParents()
{
    for(auto nextBody: mBodies) {
        if (nextBody->parentBodyName().isEmpty())
            continue;
         for(auto otherBody: mBodies) {
             if (nextBody == otherBody)
                 continue;
             if (nextBody->parentBodyName() == otherBody->ident()) {
                 nextBody->setParentBody(otherBody);
                 continue;
             }
         }
    }
}

//-------------------------------------------------------------------------------------------------
void BzModel::finalizeSetup()
{
    for(auto nextBody: mBodies)
        nextBody->finalizeSetup();
}
