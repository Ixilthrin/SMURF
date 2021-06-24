#ifndef GENG_PLANE_TERRAIN_H
#define GENG_PLANE_TERRAIN_H

#include "Terrain.h"

class PlaneTerrain : public Terrain
{
public:
    PlaneTerrain(const XMLNode & terrainXmlRootNode, int maxProxies, const btVector3 & aabbMin, const btVector3 & aabbMax);
    virtual ~PlaneTerrain();

private:

};

#endif
