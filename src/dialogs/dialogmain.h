#ifndef DIALOGMAIN_H
#define DIALOGMAIN_H

#include <QWidget>

namespace Ui {
class DialogMain;
}

class DialogMain : public QWidget
{
    Q_OBJECT

public:
    explicit DialogMain(QWidget *parent = nullptr);
    ~DialogMain();

signals:
    void selectScenario();

private:
    Ui::DialogMain *ui;
};

#endif // DIALOGMAIN_H
