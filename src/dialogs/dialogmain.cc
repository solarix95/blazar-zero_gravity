#include <QPushButton>
#include <QApplication>
#include "dialogmain.h"
#include "ui_dialogmain.h"

//-------------------------------------------------------------------------------------------------
DialogMain::DialogMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogMain)
{
    ui->setupUi(this);

    connect(ui->btnQuit, &QPushButton::clicked, qApp, &QApplication::quit);

    connect(ui->btnNew,  &QPushButton::clicked, this, [this]() {
        emit selectScenario();
        this->deleteLater();
    });

    connect(ui->btnNew,  &QPushButton::pressed, this, [this]() {
        emit selectScenario();
        this->deleteLater();
    });

    connect(ui->btnEnter, &QPushButton::clicked, this, [this]() {
        this->deleteLater();
    });

    ui->btnQuit->setFocus();
}

DialogMain::~DialogMain()
{
    delete ui;
}
