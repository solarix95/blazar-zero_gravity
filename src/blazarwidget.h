#ifndef BLAZARWIDGET_H
#define BLAZARWIDGET_H

#include <QWidget>
#include "assets/bzassets.h"
#include "ui/bzcamera.h"

class QGridLayout;
class Qtr3dWidget;
class BzModel;
class BzActions;
class BzHeadupDisplay;
class BlazarWidget  : public QWidget
{
public:
    BlazarWidget();

    void init(BzActions &actions);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    void initDisplay();
    void initModel();
    void initRendering();
    void initMenuWidget();

    void setup3D();
    void setupMainDialog();
    void setupNewDialog();
    void showDialog(QWidget *dialog);

    BzAssets     mAssets;
    BzActions   *mActions;
    BzCamera     mCamera;
    Qtr3dWidget *m3DDisplay;
    BzHeadupDisplay *mHeadup;
    QWidget     *mMenuDisplay;
    QWidget     *mCenterWidget;
    QGridLayout *mCenterLayout;
    BzModel     *mModel;
};

#endif // BLAZARWIDGET_H
