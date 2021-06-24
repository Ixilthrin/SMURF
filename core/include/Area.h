#ifndef GENG_AREA_H
#define GENG_AREA_H

#include <string>
#include <map>
#include <vector>
#include "btBulletDynamicsCommon.h"
#include "PhysicsSimulation.h"

class Avatar;
class Scene;
class Camera;
class Entity;
class GraphicsSystem;
class PhysicsSimulation;

class Area
{
public:
    Area(std::string thePhysicsName); // Simple area with y=0 collision plane
    Area(std::string thePhysicsName, const std::string & terrainXml, int maxProxies);
    ~Area();

    Avatar * getAvatar() { return physicsSimulation->getAvatar(); }
    void createTerrainObject(
                             float x, 
                             float y, 
                             float z, 
                             float yaw, 
                             const std::string & colladaFile, 
                             float renderScale,
                             float renderXOffset,
                             float renderYOffset,
                             float renderZOffset,
                             float renderYawOffset, 
                             int collisionShapeId, 
                             float collisionShapeXOffset,
                             float collisionShapeYOffset,
                             float collisionShapeZOffset,
                             float collisionShapeYawOffset);
    void update(float deltaSeconds);

    // Return pointer to entity picked by mouse click or NULL if no object picked.
    Entity * pick(int screenX, int screenY);
    void createCollisionBox(int id, float xHalfExtent, float yHalfExtent, float zHalfExtent);
    Entity * createEntity(const btVector3 & position, float yaw);
    void destroyEntity(Entity * entity);
    PhysicsSimulation * getPhysicsSimulation() { return physicsSimulation; }

private:
    std::string physicsName;
    std::vector<Entity *> entities;

    // graphics
    GraphicsSystem * graphicsSystem;
    typedef unsigned int COLOR_ID;
    std::map<COLOR_ID, Entity *> colorIdMap;
//    Scene * scene;                Should use this later on when multiple scenes are used.

    // physics
    PhysicsSimulation * physicsSimulation;

    friend class Entity; // To allow Entity to modify colorIdMap.
    
    void createTerrainObject(
        const btTransform & worldTransform,
        const std::string & colladaFile, 
        float renderScale,
        const btTransform & renderOffset,
        btCollisionShape * collisionShape,
        const btTransform & collisionShapeOffset);
};

#endif
