#ifndef BZMODEL_H
#define BZMODEL_H

#include <QObject>
#include <QList>

class BzBody;
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
    void process(int ms);

    inline QList<BzBody*> bodies() const { return mBodies; }

    // Attributes
    inline double  worldRadius()  const { return mWorldRadius;  }
    inline QString worldTexture() const { return mWorldTexture; }

signals:
    void aboutToReset();
    void loaded();

private:
    void processLeapfrog(int ms);
    void updateBodyVectors(int ms);
    void updateBodyPositions(int ms);

    QList<BzBody*> mBodies;

    // Representation
    double  mWorldRadius;
    QString mWorldTexture;
};

#endif // BZMODEL_H
