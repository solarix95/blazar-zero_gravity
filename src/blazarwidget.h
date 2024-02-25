#ifndef BLAZARWIDGET_H
#define BLAZARWIDGET_H

#include <QWidget>
#include "assets/bzassets.h"

class Qtr3dWidget;
class BzModel;
class BlazarWidget  : public QWidget
{
public:
    BlazarWidget();

    void init();

private:
    void initDisplay();
    void initModel();
    void initRendering();

    BzAssets     mAssets;
    Qtr3dWidget *m3DDisplay;
    BzModel     *mModel;
};

#endif // BLAZARWIDGET_H
