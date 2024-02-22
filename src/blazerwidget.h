#ifndef BLAZERWIDGET_H
#define BLAZERWIDGET_H

#include <QWidget>

class Qtr3dWidget;
class BzModel;
class BlazerWidget  : public QWidget
{
public:
    BlazerWidget();

    void init();

private:
    void initDisplay();
    void initModel();

    Qtr3dWidget *m3DDisplay;
    BzModel     *mModel;
};

#endif // BLAZERWIDGET_H
