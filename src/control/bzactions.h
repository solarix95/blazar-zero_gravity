#ifndef BZACTIONS_H
#define BZACTIONS_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include <QList>
#include <QPair>
#include <QKeySequence>

using BzSimpleCallback = std::function<void()>;


class BzActions : public QObject
{
    Q_OBJECT
public:
    BzActions();
    virtual ~BzActions();

    void bindKeySource(QWidget *widget);
    void bind(const QString &key, const QString &actionName);

    void onSimpleEvent(const QString &action, QObject *context, BzSimpleCallback cb);

signals:
    void activated(const QString &actionName);
    void activatedValue(const QString &actionName, int value);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

private:
    void unregisterContext(QObject *obj);

    using SimpleContext = QList<QPair<QObject*,BzSimpleCallback>>;

    QMap<QKeySequence, QString>   mKeyActions;
    QMap<QString, SimpleContext>  mSimpleKeyContexts;


};

#endif // BZACTIONS_H
