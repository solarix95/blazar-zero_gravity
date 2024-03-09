#include <QPainter>
#include <QTimer>
#include <QPointF>
#include <QString>
#include "model/bzmodel.h"
#include "bzheadupdisplay.h"

//-------------------------------------------------------------------------------------------------
BzHeadupDisplay::BzHeadupDisplay(QWidget *parent)
 : QWidget(parent)
 , mModel(nullptr)
{
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [this]() {
       this->update();
    });
    t->start(1000/20);
}

//-------------------------------------------------------------------------------------------------
void BzHeadupDisplay::setModel(BzModel *model)
{
    mModel = model;
}

//-------------------------------------------------------------------------------------------------
void BzHeadupDisplay::paintEvent(QPaintEvent *event)
{
    if (!mModel)
        return;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(Qt::yellow);

    p.drawText(QPointF(10,10),QString::number(mModel->timeScale(),'f',1));
    p.drawText(QPointF(10,30),time2String(mModel->missionTimeSecs()));
}

//-------------------------------------------------------------------------------------------------
QString BzHeadupDisplay::time2String(qint64 secs)
{
    const double SECONDS_PER_MINUTE = 60;
    const double SECONDS_PER_HOUR = 3600; // 60 * 60
    const double SECONDS_PER_DAY = 86400; // 24 * 3600
    const double SECONDS_PER_MONTH = 2629746; // Approx. 30.44 * 86400
    const double SECONDS_PER_YEAR = 31556952; // Approx. 365.25 * 86400

    auto missionTime = secs;
    int years = secs / SECONDS_PER_YEAR;
    secs %= (long long)SECONDS_PER_YEAR;

    int months = secs / SECONDS_PER_MONTH;
    secs %= (long long)SECONDS_PER_MONTH;

    int days = secs / SECONDS_PER_DAY;
    secs %= (long long)SECONDS_PER_DAY;

    int hours = secs / SECONDS_PER_HOUR;
    secs %= (long long)SECONDS_PER_HOUR;

    int minutes = secs / SECONDS_PER_MINUTE;
    secs %= (long long)SECONDS_PER_MINUTE;

    if (missionTime < 120)
        return QString("%1s").arg(secs);

    if (missionTime < SECONDS_PER_DAY)
        return QString("%1h:%2m:%3s").arg(hours).arg(minutes).arg(secs);

    if (missionTime < SECONDS_PER_MONTH)
        return QString("%1d %2h:%3m:%4s").arg(days).arg(hours).arg(minutes).arg(secs);

    return QString("%1y:%2mo:%3d %4h:%5m:%6s").arg(years).arg(months).arg(days).arg(hours).arg(minutes).arg(secs);
}
