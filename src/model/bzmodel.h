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
    void process(int ms);

    inline QList<BzBody*> bodies()     const { return mBodies; }
    inline int            timeScale()  const { return mTargetTimeScale;  }

    // Rendering Attributes
    inline double  worldRadius()  const { return mWorldRadius;  }
    inline QString worldTexture() const { return mWorldTexture; }

signals:
    void aboutToReset();
    void loaded();
    void bodyActivated(BzBody *body);
    void targetTimeScaleChanged(int newScale);
    void timeScaleChanged(float newScale);

private:
    void processLeapfrog(int ms);
    void updateBodyVectors(int ms);
    void updateBodyPositions(int ms);

    void deserializePlanets(const QList<BzConfig> &planetConfig);
    void deserializeBodies(const QList<BzConfig> &bodiesConfig);

    QList<BzBody*>  mCelestials; // N-Body-Simulation, "Planets"
    QList<BzBody*>  mBodies;     // Complete List of all actors
    BzBody         *mActiveBody;
    int             mTimeScale;

    float           mTargetTimeScale;
    float           mCurrentTimeScale;
    float           mDeltaTimeScale;

    // Representation
    double  mWorldRadius;
    QString mWorldTexture;
};

#endif // BZMODEL_H
