#include "blazerwidget.h"

#include "model/bzmodel.h"
#include "model/bzbody.h"
//-------------------------------------------------------------------------------------------------
BlazerWidget::BlazerWidget()
 : QWidget()
 , mModel(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void BlazerWidget::init()
{
    mModel = new BzModel();

    auto *sun   = new BzBody(1.989e30);
    auto *earth = new BzBody(5.972e24);

    earth->setGlobalPos({149.6e9,0.0,0.0});
    earth->setSpeedKmS({0.0,0.0,29.78});

    mModel->addBody(sun);
    mModel->addBody(earth);
}
