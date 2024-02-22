#include <QVBoxLayout>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dassets.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>

#include "blazerwidget.h"

#include "model/bzmodel.h"
#include "model/bzbody.h"

//-------------------------------------------------------------------------------------------------
BlazerWidget::BlazerWidget()
 : QWidget()
 , m3DDisplay(nullptr)
 , mModel(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void BlazerWidget::init()
{
    initDisplay();
    initModel();
}

//-------------------------------------------------------------------------------------------------
void BlazerWidget::initDisplay()
{
    auto *layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(m3DDisplay = new Qtr3dWidget(Qtr3dWidget::MSAA4));
    setGeometry(50,50,800,600);
    setWindowTitle("Blazar - Zero-Gravity Simulator");
}

//-------------------------------------------------------------------------------------------------
void BlazerWidget::initModel()
{
    mModel = new BzModel();

    auto *sun   = new BzBody(1.989e30);
    auto *earth = new BzBody(5.972e24);
    earth->setIdent("earth");

    earth->setGlobalPos({149.6e9,0.0,0.0});
    earth->setSpeedKmS({0.0,0.0,29.78});

    mModel->addBody(sun);
    mModel->addBody(earth);

    connect(&m3DDisplay->assets()->loop(),&Qtr3dFpsLoop::step, this, [this](float ms, float normalizedSpeed) {
        mModel->process(ms);
    });

    m3DDisplay->assets()->loop().setFps(50);
    m3DDisplay->assets()->loop().setSpeed(1000000);

    connect(m3DDisplay, &Qtr3dWidget::initialized, this, [this](){
        auto *mesh = m3DDisplay->createMesh();

        Qtr3d::meshBySphere(*mesh,640,QImage("gamedata/2k_sun.jpg")); // by Webtreats: https://www.flickr.com/photos/webtreatsetc/5643615278/
        mesh->material() = Qtr3d::Material(1,0,0);
        auto *sun1 = m3DDisplay->createState(mesh);
        sun1->setScale(696340);
        sun1->setRotation({90,0,0});

        m3DDisplay->primaryLightSource()->setPos({0,0,0});
        m3DDisplay->primaryLightSource()->setAmbientStrength(0.1);
        m3DDisplay->camera()->setFov(45,1,100000000);
        m3DDisplay->camera()->setPos(0,0,-3*696340);
    });

}
