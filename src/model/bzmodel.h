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
    void addPart(BzPart *part);
    void setTimeScale(int gamespeed);
    void incTimeScale(int diff);
    void process(int ms);

    inline QList<BzBody*> bodies()     const { return mBodies; }
    inline QList<BzPart*> parts()      const { return mParts;  }
    inline int            timeScale()  const { return mTimeScale;  }

    // Attributes
    inline double  worldRadius()  const { return mWorldRadius;  }
    inline QString worldTexture() const { return mWorldTexture; }

signals:
    void aboutToReset();
    void loaded();
    void timeScaleChanged(int newScale);

private:
    void processLeapfrog(int ms);
    void updateBodyVectors(int ms);
    void updateBodyPositions(int ms);

    void deserializePlanets(const QList<BzConfig> &planetConfig);
    void deserializeBodies(const QList<BzConfig> &bodiesConfig);

    QList<BzBody*>  mBodies; // N-Body-Simulation, "Planets"
    QList<BzPart*>  mParts;
    int             mTimeScale;

    // Representation
    double  mWorldRadius;
    QString mWorldTexture;
};

#endif // BZMODEL_H
