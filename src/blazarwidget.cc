#include <QVBoxLayout>
#include <QShortcut>
#include <QKeySequence>
#include <QResizeEvent>
#include <QGridLayout>
#include <QSpacerItem>
#include <QDebug>
#include <math.h>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dassets.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>

#include "blazarwidget.h"

#include "assets/bzconfigs.h"
#include "assets/bztextures.h"
#include "assets/bzfilebuffer.h"
#include "model/bzmodel.h"
#include "model/bzbody.h"
#include "model/bzpart.h"
#include "model/bzplanet.h"

#include "control/bzactions.h"
#include "dialogs/dialogmain.h"
#include "dialogs/dialogscenarioselection.h"

//-------------------------------------------------------------------------------------------------
BlazarWidget::BlazarWidget()
 : QWidget()
 , mActions(nullptr)
 , m3DDisplay(nullptr)
 , mMenuDisplay(nullptr)
 , mCenterWidget(nullptr)
 , mModel(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::init(BzActions &actions)
{
    mAssets.init("gamedata");
    mActions = &actions;
    mActions->bindKeySource(this);

    initDisplay();

    connect(new QShortcut(QKeySequence("F11"),this),&QShortcut::activated, this, [this]() {
       if (isFullScreen())
           showNormal();
       else
           showFullScreen();
    });

    connect(new QShortcut(QKeySequence("Esc"),this),&QShortcut::activated, this, [this]() {
        setupMainDialog();
    });
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::resizeEvent(QResizeEvent *event)
{
    m3DDisplay->setGeometry(QRect(QPoint(0,0),event->size()));
    mMenuDisplay->setGeometry(QRect(QPoint(0,0),event->size()));
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initDisplay()
{
    m3DDisplay  = new Qtr3dWidget(Qtr3dWidget::MSAA16, this);
    mMenuDisplay = new QWidget(this);

    m3DDisplay->stackUnder(mMenuDisplay);

    setGeometry(50,50,800,600);
    setWindowTitle("Blazar - Zero-Gravity Simulator");

    connect(m3DDisplay, &Qtr3dWidget::initialized, this, [this](){
        setup3D();
        initModel();
    });
    initMenuWidget();
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initModel()
{
    mModel = new BzModel();
    connect(mModel, &BzModel::loaded, this, [this]() { initRendering(); });

    mActions->onSimpleEvent("incTimeScale",this,[this]() {
        mModel->incTimeScale(+1);
    });
    mActions->onSimpleEvent("decTimeScale",this,[this]() {
        mModel->incTimeScale(-1);
    });
    mActions->onSimpleEvent("nextBody",this,[this]() {
        mModel->activateNextBody();
    });

    connect(&m3DDisplay->assets()->loop(),&Qtr3dFpsLoop::step, this, [this](float ms, float normalizedSpeed) {
        mModel->process(ms);
        m3DDisplay->update();
    });
    m3DDisplay->assets()->loop().setFps(50);

    connect(mModel,&BzModel::timeScaleChanged, this, [this](float ts) {
            m3DDisplay->assets()->loop().setSpeed(ts);
    });
    connect(mModel,&BzModel::bodyActivated, this, [this](BzBody *b) {
        mCamera.follow(b);
    });

    auto scenario = mAssets.scenarios().byName("Softwaretest Mars");
    mModel->deserialize(scenario);
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initRendering()
{
    Q_ASSERT(m3DDisplay);

    double distance = 0;
    for(auto *body: mModel->bodies()) {
        switch(body->type()) {
        case BzBody::CelestialType: {
            BzPlanet *p = dynamic_cast<BzPlanet*>(body);
            Q_ASSERT(p);
            auto *mesh = m3DDisplay->createMesh();
            Qtr3d::meshBySphere(*mesh,640,640,p->radius(),true,mAssets.textures()[p->textureName()]);
            mesh->material() = Qtr3d::Material(1,0,0);
            auto *planetRepresentation = m3DDisplay->createState(mesh);
            body->setRepresentation(planetRepresentation);
            distance = (p->globalPos().length() + p->radius() * 3) > distance ? (2*p->globalPos().length() + p->radius() * 3): distance;
        } break;
         case BzBody::PartType: {
            BzPart *p = dynamic_cast<BzPart*>(body);
            Q_ASSERT(p);
            auto *model = m3DDisplay->createModel();
            Qtr3d::modelByFileBuffer(*model,p->displayInfo().modelName, mAssets.models()[p->displayInfo().modelName]);
            model->material() = Qtr3d::Material(1,0,0);
            auto *partRepresentation = m3DDisplay->createState(model);
            double radius = model->radius();
            Q_ASSERT(radius > 0);
            partRepresentation->setScale(10/radius);
            p->setCollisionRadius(10);
            p->setRepresentation(partRepresentation);
        } break;
        }
    }

    // sun1->setRotation({90,0,0});

    m3DDisplay->primaryLightSource()->setPos({10*distance,0.0,10*distance});
    m3DDisplay->primaryLightSource()->setAmbientStrength(0);

    m3DDisplay->camera()->setPos(0,0,distance);
    m3DDisplay->camera()->lookAt({0,0,0},{0,1,0});
    m3DDisplay->setDefaultLighting(Qtr3d::FlatLighting);

    if (!mModel->worldTexture().isEmpty()) {
        auto mesh = m3DDisplay->createMesh();
        Qtr3d::meshBySphere(*mesh,640,640,mModel->worldRadius(),false,mAssets.textures()[mModel->worldTexture()]);
        mesh->material() = Qtr3d::Material(1,0,0);
        auto *milkyway = m3DDisplay->createState(mesh);
        connect(mModel, &BzModel::aboutToReset, milkyway, &QObject::deleteLater);
    }

    /*
    static float pulsating = 0;
    connect(&m3DDisplay->assets()->loop(), &Qtr3dFpsLoop::stepDone, this, [this, mesh, sun1]() {
       pulsating += 0.001;
        mesh->material() = Qtr3d::Material(1 + (0.5 * sin(pulsating)),0,0);
        sun1->setRotation({0.0,pulsating*20,0.0});
    });

    connect(&m3DDisplay->assets()->loop(), &Qtr3dFpsLoop::stepDone, this, [this]() {
       m3DDisplay->update();
    });

    */
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initMenuWidget()
{
    mCenterLayout = new QGridLayout(mMenuDisplay);

    mCenterLayout->addItem(new QSpacerItem(20, 160, QSizePolicy::Minimum, QSizePolicy::Expanding), 2, 1, 1, 1);
    mCenterLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum) , 1, 0, 1, 1);
    mCenterLayout->addItem(new QSpacerItem(20, 161, QSizePolicy::Minimum, QSizePolicy::Expanding), 0, 1, 1, 1);
    mCenterLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum), 1, 2, 1, 1);
    // gridLayout->addWidget(mCenterWidget = new QWidget(this), 1, 1, 1, 1);
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::setup3D()
{
    m3DDisplay->camera()->setFov(45,1,100000000);
    mCamera.setCamera(m3DDisplay->camera());
    m3DDisplay->assets()->loop().setSpeed(1);

    connect(&m3DDisplay->assets()->loop(), &Qtr3dFpsLoop::step, this, [this](int ms, float normalizedSpeed) {
        mCamera.process(ms);
    });
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::setupMainDialog()
{
    auto *dlg = new DialogMain();
    connect(dlg, &DialogMain::selectScenario,this, &BlazarWidget::setupNewDialog);
    showDialog(dlg);
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::setupNewDialog()
{
    auto *dlg = new DialogScenarioSelection(mAssets);
    connect(dlg, &DialogScenarioSelection::selectedItem,this, [this](const QString &name) {
        auto scenario = mAssets.scenarios().byName(name);
        mModel->deserialize(scenario);
    });

    showDialog(dlg);
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::showDialog(QWidget *dialog)
{
    if (mCenterWidget) {
        mCenterLayout->removeWidget(mCenterWidget);
        mCenterWidget->deleteLater();
    }

    mCenterLayout->addWidget(dialog, 1, 1, 1, 1);
    connect(dialog, &QObject::destroyed, this, [this,dialog]() {
        if (mCenterWidget == dialog)
            mCenterWidget = nullptr;
    });
    mCenterWidget = dialog;
}
