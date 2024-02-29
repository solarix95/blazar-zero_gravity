
#include "assets/bzassets.h"
#include "assets/bzconfigs.h"
#include "dialogscenarioselection.h"
#include "ui_dialogscenarioselection.h"

//-------------------------------------------------------------------------------------------------
DialogScenarioSelection::DialogScenarioSelection(const BzAssets &assets, QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::DialogScenarioSelection)
  , mAssets(assets)
{
    ui->setupUi(this);

    QStringList names  = mAssets.scenarios().names();

    ui->listWidget->addItems(names);

    connect(ui->listWidget, &QListWidget::activated, this, [this](const QModelIndex &index) {
        emit selectedItem(index.data().toString());
        this->deleteLater();
    });

    if (names.count() > 0)
        ui->listWidget->selectionModel()->select(ui->listWidget->model()->index(0,0),QItemSelectionModel::Select);

    ui->listWidget->setFocus();
}

//-------------------------------------------------------------------------------------------------
DialogScenarioSelection::~DialogScenarioSelection()
{
    delete ui;
}
