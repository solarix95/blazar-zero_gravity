#ifndef BZASSETS_H
#define BZASSETS_H

#include <QObject>

class BzTextures;
class BzAssets
{
public:
    BzAssets();

    void init(const QString &basedir);

    const BzTextures &textures() const;

private:
    BzTextures *mTextures;
};

#endif // BZASSETS_H
