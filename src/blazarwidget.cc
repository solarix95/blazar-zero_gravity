#include <QVBoxLayout>
#include <QDebug>
#include <math.h>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dassets.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>

#include "blazarwidget.h"

#include "assets/bztextures.h"
#include "model/bzmodel.h"
#include "model/bzbody.h"

//-------------------------------------------------------------------------------------------------
BlazarWidget::BlazarWidget()
 : QWidget()
 , m3DDisplay(nullptr)
 , mModel(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::init()
{
    mAssets.init("gamedata");

    initDisplay();
    initModel();
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initDisplay()
{
    auto *layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(m3DDisplay = new Qtr3dWidget(Qtr3dWidget::MSAA16));
    setGeometry(50,50,800,600);
    setWindowTitle("Blazar - Zero-Gravity Simulator");

    connect(m3DDisplay, &Qtr3dWidget::initialized, this, [this](){
        initModel();
        initRendering();
    });
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initModel()
{
    mModel = new BzModel();

    auto *sun   = new BzBody(1.989e30);
    auto *earth = new BzBody(5.972e24);
    earth->setIdent("earth");

    earth->setGlobalPos({149.6e9,0.0,0.0});
    earth->setSpeedKmS({0.0,0.0,29.78});

    mModel->addBody(sun);
    mModel->addBody(earth);
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initRendering()
{
    Q_ASSERT(m3DDisplay);
    auto *mesh = m3DDisplay->createMesh();

    Qtr3d::meshBySphere(*mesh,640,640,696340,true,mAssets.textures()["8k_moon.jpg"]);
    mesh->material() = Qtr3d::Material(1,0,0);
    auto *sun1 = m3DDisplay->createState(mesh);

    // sun1->setRotation({90,0,0});

    m3DDisplay->primaryLightSource()->setPos({0,0,0});
    m3DDisplay->primaryLightSource()->setAmbientStrength(0.1);
    m3DDisplay->camera()->setFov(45,1,100000000);
    m3DDisplay->camera()->setPos(0,0,-3*696340);

    m3DDisplay->setDefaultLighting(Qtr3d::PhongLighting);

    static double pulsating = 0;
    connect(&m3DDisplay->assets()->loop(), &Qtr3dFpsLoop::stepDone, this, [this, mesh, sun1]() {
       pulsating += 0.001;
        mesh->material() = Qtr3d::Material(1 + (0.5 * sin(pulsating)),0,0);
        sun1->setRotation({0.0,pulsating*20,0.0});
    });

    connect(&m3DDisplay->assets()->loop(), &Qtr3dFpsLoop::stepDone, this, [this]() {
       m3DDisplay->update();
    });

    mesh = m3DDisplay->createMesh();
    Qtr3d::meshBySphere(*mesh,640,640,69634000,false,mAssets.textures()["8k_stars_milky_way.jpg"]);
    mesh->material() = Qtr3d::Material(1,0,0);
    auto *milkyway = m3DDisplay->createState(mesh);

    connect(&m3DDisplay->assets()->loop(),&Qtr3dFpsLoop::step, this, [this](float ms, float normalizedSpeed) {
        mModel->process(ms);
    });

    m3DDisplay->assets()->loop().setFps(50);
    m3DDisplay->assets()->loop().setSpeed(1000000);
}
