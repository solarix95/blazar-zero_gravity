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
#include <libqtr3d/extras/qtr3dfreecameracontroller.h>
#include <libqtr3d/extras/qtr3dwidgetinputcontroller.h>
#include "blazarwidget.h"

#include "assets/bzconfigs.h"
#include "assets/bztextures.h"
#include "assets/bzfilebuffer.h"
#include "model/bzmodel.h"
#include "model/bzbody.h"
#include "model/bzpart.h"
#include "model/bzplanet.h"
#include "types/bzunits.h"

#include "control/bzactions.h"
#include "dialogs/dialogmain.h"
#include "dialogs/dialogscenarioselection.h"
#include "ui/bzheadupdisplay.h"

//-------------------------------------------------------------------------------------------------
BlazarWidget::BlazarWidget()
 : QWidget()
 , mActions(nullptr)
 , m3DDisplay(nullptr)
 , mHeadup(nullptr)
 , mMenuDisplay(nullptr)
 , mCenterWidget(nullptr)
 , mModel(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::init(BzActions &actions)
{
    mAssets.init("gamedata", "userdata");
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

    mActions->onSimpleEvent("nextCamera",this,[this]() {
        mCamera.nextMode();
    });
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::resizeEvent(QResizeEvent *event)
{
    m3DDisplay->setGeometry(QRect(QPoint(0,0),event->size()));
    mHeadup->setGeometry(QRect(QPoint(0,0),event->size()));
    mMenuDisplay->setGeometry(QRect(QPoint(0,0),event->size()));
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initDisplay()
{
    m3DDisplay   = new Qtr3dWidget((Qtr3dWidget::Options)Qtr3dWidget::MSAA16 | Qtr3dWidget::OriginRebasing, this);
    mHeadup      = new BzHeadupDisplay(this);
    mMenuDisplay = new QWidget(this);

    m3DDisplay->stackUnder(mHeadup);
    mHeadup->stackUnder(mMenuDisplay);
    // mHeadup->stackUnder(m3DDisplay);
    // mMenuDisplay->stackUnder(m3DDisplay);

    setGeometry(50,50,800,600);
    setWindowTitle("Blazar - Zero-Gravity Simulator");

    connect(m3DDisplay, &Qtr3dWidget::initialized, this, [this](){
        setup3D();
        initModel();
    });
    initMenuWidget();

    auto *inputController = new Qtr3dWidgetInputController(mMenuDisplay);
    connect(inputController, &Qtr3dWidgetInputController::mouseWheel, this, [this](float delta) {
       mCamera.zoom(delta);
    });
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


    mHeadup->setModel(mModel);
    connect(&m3DDisplay->assets()->loop(),&Qtr3dFpsLoop::step, this, [this](float ms, float normalized, int real) {
        mModel->process(ms,normalized, real);
        mCamera.process(real);
        m3DDisplay->update();
    });
    m3DDisplay->assets()->loop().setFps(50);

    connect(mModel,&BzModel::timeScaleChanged, this, [this](float ts) {
            m3DDisplay->assets()->loop().setSpeed(ts);
    });
    connect(mModel,&BzModel::bodyActivated, this, [this](BzBody *b) {
        mCamera.follow(b);
    });

    // auto scenario = mAssets.scenarios().byName("Softwaretest ISS Only");
    // auto scenario = mAssets.scenarios().byName("Softwaretest ISS Only");
    auto scenario = mAssets.scenarios().byName("Softwaretest Earth/ISS");
    mModel->deserialize(scenario);
    // m3DDisplay->camera()->setFov(45,100,mModel->worldRadius()*2);
}

//-------------------------------------------------------------------------------------------------
void BlazarWidget::initRendering()
{
    Q_ASSERT(m3DDisplay);

    Qtr3dGeometryState *primaryLightSource = nullptr; // the sun

    for(auto *body: mModel->bodies()) {
        switch(body->type()) {
        case BzBody::CelestialType: {
            BzPlanet *p = dynamic_cast<BzPlanet*>(body);
            Q_ASSERT(p);
            auto *mesh = m3DDisplay->createMesh();
            auto texture = mAssets.textures()[p->textureName()];
            if (texture.isNull()) {
                qDebug() << "Invalid Texture: " << p->textureName();
                mesh->deleteLater();
                continue;
            }
            Qtr3d::meshBySphere(*mesh,512,1024,BzUnit::meters2ogl(p->collisionRadius()) ,true,mAssets.textures()[p->textureName()].mirrored(true,false),true);
            auto *planetRepresentation = m3DDisplay->createState(mesh);
            mesh->setRenderOption(Qtr3d::ZOrderOption);
            body->setRepresentation(planetRepresentation);

            if (p->tags().contains("lightsource")) {
                primaryLightSource = planetRepresentation;
                primaryLightSource->setLightingType(Qtr3d::NoLighting);
                mesh->material().ambient().setStrength(1);

            }

        } break;
         case BzBody::PartType: {
            BzPart *p = dynamic_cast<BzPart*>(body);
            Q_ASSERT(p);
            auto *model = m3DDisplay->createModel();
            Qtr3d::modelByFileBuffer(*model,
                                     mAssets.models().absolutFileName(p->displayInfo().modelName),
                                     mAssets.models()[p->displayInfo().modelName]);
            model->setFaceOrientation(Qtr3d::CounterClockWise);
            auto *partRepresentation = m3DDisplay->createState(model);
            double radius = p->collisionRadius();
            if (radius > 0)
                partRepresentation->setScale(BzUnit::meters2ogl(radius/model->radius()));
            else
                radius = partRepresentation->radius();

            p->setCollisionRadius(radius);
            p->setRepresentation(partRepresentation);

        } break;
        }
    }

    // sun1->setRotation({90,0,0});

    // m3DDisplay->camera()->setFov(60,0.01,10000);
    m3DDisplay->camera()->setFov(50,0.05,10000);
    // m3DDisplay->camera()->lookAt({3*63710,3*63710,3*63710},{0,0,0},{0,1,0});
    // m3DDisplay->camera()->lookAt({2, 0, -214578},{0,0,0},{0,1,0});

    // m3DDisplay->primaryLightSource()->setPos({10*distance,0.0,10*distance});
    // m3DDisplay->primaryLightSource()->setAmbientStrength(0);

    // m3DDisplay->camera()->setPos(0,0,distance);
    // m3DDisplay->camera()->lookAt({0,0,0},{0,1,0});

    if (primaryLightSource) {
        connect(primaryLightSource, &Qtr3dGeometryState::updated, this, [this, primaryLightSource](){
            m3DDisplay->primaryLightSource()->setPos(primaryLightSource->pos());
        });
        m3DDisplay->setDefaultLighting(Qtr3d::PhongLighting);
    } else
        m3DDisplay->setDefaultLighting(Qtr3d::NoLighting);

    if (!mModel->skyBox().texture.isEmpty()) {
        auto mesh = m3DDisplay->createMesh();
        Qtr3d::meshBySphere(*mesh,640,640,mModel->skyBox().radius,false,mAssets.textures()[mModel->skyBox().texture]);
        mesh->setRenderOption(Qtr3d::BackgroundOption); // TODO: Render tatsächlich in the background (mit shader!!!=
        auto *milkyway = m3DDisplay->createState(mesh);
        milkyway->setLightingType(Qtr3d::NoLighting);
        connect(mModel, &BzModel::aboutToReset, milkyway, &QObject::deleteLater);

        connect(m3DDisplay->camera(), &Qtr3dCamera::positionChanged, milkyway, [milkyway, this]() {
            milkyway->setPos(m3DDisplay->camera()->pos());
        });
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
    mCamera.setCamera(m3DDisplay->camera());
    // new Qtr3dFreeCameraController(m3DDisplay);
    m3DDisplay->assets()->loop().setSpeed(1);
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
