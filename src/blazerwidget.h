#ifndef BLAZERWIDGET_H
#define BLAZERWIDGET_H

#include <QWidget>

class BzModel;
class BlazerWidget  : public QWidget
{
public:
    BlazerWidget();

    void init();

private:

    BzModel *mModel;
};

#endif // BLAZERWIDGET_H
