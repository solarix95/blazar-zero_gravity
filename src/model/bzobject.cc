#include "bzobject.h"

//-------------------------------------------------------------------------------------------------
BzObject::BzObject()
 : QObject()
{
}

//-------------------------------------------------------------------------------------------------
bool BzObject::deserialize(const BzConfig &config)
{
    config.parameter("name",mIdent);
    config.parameter("tags",mTags);
    return true;
}
