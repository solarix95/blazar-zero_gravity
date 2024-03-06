#ifndef BZHEADUPDISPLAY_H
#define BZHEADUPDISPLAY_H

#include <QWidget>

class BzModel;
class BzHeadupDisplay : public QWidget
{
public:
    BzHeadupDisplay(QWidget *parent);
    virtual ~BzHeadupDisplay() = default;

    void setModel(BzModel *model);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    static QString time2String(qint64 secs);
    BzModel *mModel;
};

#endif // BZHEADUPDISPLAY_H
