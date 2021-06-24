#include "PlaneTerrain.h"
//#include "Physics.h"

PlaneTerrain::PlaneTerrain(const XMLNode & terrainXmlRootNode, int maxProxies, const btVector3 & aabbMin, const btVector3 & aabbMax) :
    Terrain(maxProxies, aabbMin, aabbMax)
{
//    Physics * physics = Physics::getInstance();
//    physics->createSimpleGround();
}

PlaneTerrain::~PlaneTerrain()
{
}
