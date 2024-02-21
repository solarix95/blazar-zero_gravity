#ifndef BZMODEL_H
#define BZMODEL_H

#include <QObject>
#include <QList>

class BzBody;
class BzModel : public QObject
{
public:
    BzModel();

    void addBody(BzBody *body);
    void process(int ms);

private:
    void processLeapfrog(int ms);
    void updateBodyVectors(int ms);
    void updateBodyPositions(int ms);

    QList<BzBody*> mBodies;
};

#endif // BZMODEL_H
