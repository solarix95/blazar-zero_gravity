#ifndef BZOBJECT_H
#define BZOBJECT_H

#include <QObject>
#include <QString>

class BzObject : public QObject
{
public:
    BzObject();

    inline void setIdent(const QString &ident) { mIdent = ident; }
    inline QString ident() const               { return mIdent;  }

private:
    QString mIdent;
};

#endif // BZOBJECT_H
