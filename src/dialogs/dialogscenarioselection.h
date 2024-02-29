#ifndef DIALOGSCENARIOSELECTION_H
#define DIALOGSCENARIOSELECTION_H

#include <QWidget>

namespace Ui {
class DialogScenarioSelection;
}

class BzAssets;

class DialogScenarioSelection : public QWidget
{
    Q_OBJECT

public:
    explicit DialogScenarioSelection(const BzAssets &assets, QWidget *parent = nullptr);
    ~DialogScenarioSelection();

signals:
    void selectedItem(const QString &name);

private:
    Ui::DialogScenarioSelection *ui;
    const BzAssets &mAssets;
};

#endif // DIALOGSCENARIOSELECTION_H
