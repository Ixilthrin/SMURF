#ifndef GENG_TERRAIN_H
#define GENG_TERRAIN_H

#include <string>
#include "utXMLParser.h"

class btVector3;

class Terrain
{
public:
    static Terrain * load(const std::string & terrainXml, int maxProxies);
    virtual ~Terrain();

protected:
    Terrain(int maxProxies, const btVector3 & aabbMin, const btVector3 & aabbMax);
};

#endif
