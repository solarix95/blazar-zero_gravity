#include <QFile>
#include "bzconfig.h"

//-------------------------------------------------------------------------------------------------
BzConfig::BzConfig()
{
}

//-------------------------------------------------------------------------------------------------
bool BzConfig::fromFile(const QString &fileName)
{
    QFile f(fileName);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    return fromData(f.readAll());
}

//-------------------------------------------------------------------------------------------------
bool BzConfig::fromData(const QByteArray &data)
{
    int pos = 0;
    return fromData(data,pos);
}

//-------------------------------------------------------------------------------------------------
bool BzConfig::fromData(const QByteArray &data, int &pos)
{
    enum ParserState {
        StartState,
        KeyState,
        EndState
    };

    ParserState state = StartState;

    QString key;
    QString value;
    while (state != EndState) {
        if (!skipEmpty(data,pos))
            return false;
        switch(state) {
        case StartState:
            if (data[pos] != '{')
                return false;
            pos++;
            state = KeyState;
            break;
        case KeyState:
            if (data[pos] == '{') {
                BzConfig child;
                if (!child.fromData(data,pos))
                    return false;
                mChilds << child;
                pos++;
                continue;
            }
            if (data[pos] == '}') {
                state = EndState;
                continue;
            }
            if (!getKeyValue(data,pos,key,value))
                return false;
            mParameters[key.toLower()] = value.isEmpty() ? QVariant() : QVariant(value);
        default:;
        }
    }

    return isValid();
}

//-------------------------------------------------------------------------------------------------
bool BzConfig::isValid() const
{
    return !(mParameters.isEmpty() && mChilds.isEmpty());
}

//-------------------------------------------------------------------------------------------------
QVariant BzConfig::parameter(const QString &name, const QVariant &defaultValue) const
{
    return mParameters.value(name,defaultValue);
}

//-------------------------------------------------------------------------------------------------
QList<BzConfig> BzConfig::childsByType(const QString &typeName) const
{
    return childsByAttribut("type",typeName);
}

//-------------------------------------------------------------------------------------------------
QList<BzConfig> BzConfig::childsByAttribut(const QString &attributName, const QVariant &attributValue) const
{
    QList<BzConfig> ret;
    for (const auto &child : mChilds) {
        if (child.parameter(attributName) == attributValue)
            ret << child;
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
bool BzConfig::skipEmpty(const QByteArray &data, int &pos)
{
    static QList<char> sWhiteCharacters = QList<char>() << ' ' << '\t' << '\n' << '\r';
    while (pos < data.count() && sWhiteCharacters.contains(data[pos]))
        pos++;
    return pos < data.count();
}

//-------------------------------------------------------------------------------------------------
bool BzConfig::getKeyValue(const QByteArray &data, int &pos, QString &key, QString &value)
{
    QByteArray token;
    while (pos < data.length() && data[pos] != '=')
        token += data[pos++];

    key = QString::fromUtf8(token).trimmed();
    if (key.isEmpty())
        return false;

    pos++;
    token.clear();
    while (pos < data.length() && data[pos] != '\n')
        token += data[pos++];
    value = QString::fromUtf8(token).trimmed();

    return pos < data.count();
}
