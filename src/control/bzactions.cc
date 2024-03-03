#include <QEvent>
#include <QKeyEvent>
#include "bzactions.h"

//-------------------------------------------------------------------------------------------------
BzActions::BzActions()
{
    connect(this, &BzActions::activated, this, [this](const QString &eventName) {
         if (!mSimpleKeyContexts.contains(eventName))
             return;

         const SimpleContext &context = mSimpleKeyContexts[eventName];
         for (const auto &next: context) {
             next.second();
         }
    });
}

//-------------------------------------------------------------------------------------------------
BzActions::~BzActions()
{
}

//-------------------------------------------------------------------------------------------------
void BzActions::bindKeySource(QWidget *widget)
{
    Q_ASSERT(widget);
    widget->installEventFilter(this);
}

//-------------------------------------------------------------------------------------------------
void BzActions::bind(const QString &key, const QString &actionName)
{
    mKeyActions[QKeySequence(key)] = actionName;
}

//-------------------------------------------------------------------------------------------------
void BzActions::onSimpleEvent(const QString &action, QObject *context, BzSimpleCallback cb)
{
    mSimpleKeyContexts[action].append(qMakePair(context,std::move(cb)));
}

//-------------------------------------------------------------------------------------------------
bool BzActions::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = (QKeyEvent*)(event);
        QKeySequence ks(ke->key() | ke->modifiers());
        if (mKeyActions.contains(ks))
            emit activated(mKeyActions[ks]);
    }

    return QObject::eventFilter(watched,event);
}

//-------------------------------------------------------------------------------------------------
void BzActions::unregisterContext(QObject *obj)
{
    auto keys = mSimpleKeyContexts.keys();
    for (auto key: keys) {
        auto &contexts = mSimpleKeyContexts[key];
        for (int i=0; i<contexts.count(); i++) {
            if (contexts[i].first != obj)
                continue;
            contexts.removeAt(i);
            i--;
        }
    }
}
