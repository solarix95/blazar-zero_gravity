#ifndef BLAZARWIDGET_H
#define BLAZARWIDGET_H

#include <QWidget>
#include "assets/bzassets.h"

class QGridLayout;
class Qtr3dWidget;
class BzModel;
class BlazarWidget  : public QWidget
{
public:
    BlazarWidget();

    void init();

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void initDisplay();
    void initModel();
    void initRendering();
    void initMenuWidget();

    void setupMainDialog();
    void setupNewDialog();

    BzAssets     mAssets;
    Qtr3dWidget *m3DDisplay;
    QWidget     *mMenuDisplay;
    QWidget     *mCenterWidget;
    QGridLayout *mCenterLayout;
    BzModel     *mModel;
};

#endif // BLAZARWIDGET_H
