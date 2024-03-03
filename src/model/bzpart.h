#ifndef BZPART_H
#define BZPART_H

#include "bzbody.h"

class BzPart : public BzBody
{
public:
    BzPart(double massInTons = 0.0);

    struct DisplayInfo {
        QString textureName;
        QString modelName;
    };

    inline DisplayInfo &displayInfo() { return mDisplayInfo; }

private:
    DisplayInfo mDisplayInfo;
};

#endif // BZPART_H
