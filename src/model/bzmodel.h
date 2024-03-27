#ifndef BZMODEL_H
#define BZMODEL_H

#include <QObject>
#include <QList>

class BzBody;
class BzPart;
class BzConfig;
class BzModel : public QObject
{
    Q_OBJECT
public:
    struct SkyBox {
        double  radius;
        QString texture;
        SkyBox() : radius(-1), texture("") {}
    };

    BzModel();
    virtual ~BzModel();

    void deserialize(const BzConfig &cfg);
    void reset();

    void addBody(BzBody *body);

    // User Actions
    void setTimeScale(int gamespeed);
    void incTimeScale(int diff);
    void activateNextBody();

    // Processing
    void process(float ms, float normalizedTime, int realtime);

    inline QList<BzBody*> bodies()     const      { return mBodies;            }
    inline float          timeScale()  const      { return mCurrentTimeScale;  }
    inline qint64         missionTimeSecs() const { return mMissionTime/1000;  }

    // Rendering Attributes
    inline const SkyBox  &skyBox()  const { return mSkyBox;  }

signals:
    void aboutToReset();
    void loaded();
    void bodyActivated(BzBody *body);
    void targetTimeScaleChanged(int newScale);
    void timeScaleChanged(float newScale);

private:
    void processLeapfrog(float ms);
    void updateBodyVectors(float ms);
    void updateBodyPositions(float ms);

    void deserializePlanets(const QList<BzConfig> &planetConfig);
    void deserializeBodies(const QList<BzConfig> &bodiesConfig);
    void resolveBodyParents();
    void finalizeSetup();

    QList<BzBody*>  mCelestials; // N-Body-Simulation, "Planets"
    QList<BzBody*>  mBodies;     // Complete List of all actors
    BzBody         *mActiveBody;
    int             mTimeScale;

    double          mMissionTime;
    float           mTargetTimeScale;
    float           mCurrentTimeScale;
    float           mDeltaTimeScale;

    // Representation
    SkyBox          mSkyBox;
};

#endif // BZMODEL_H
