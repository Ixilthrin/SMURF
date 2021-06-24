#ifndef GENG_ENTITY_H
#define GENG_ENTITY_H

#include "Actor.h"
#include "AIWorldObject.h"
#include "EntityController.h"
#include "LinearMath/btTransform.h"
#include "SceneNode.h"

#include <map>
#include <string>
#include <vector>

class Area;
class Body;
class btVector3;
class Drawable;
class EntitySystem;
class SceneNode;
struct XMLNode;

// Creation and destruction of these objects should be done in the same thread,
// otherwise you need to synchronize access to allocatedColorIds.
class Entity : public AIWorldObject
{
public:
    bool isPickable() const { return sceneNode->getColorId() != 0; }
	void setPickable(bool pickable);
    SceneNode * getSceneNode() { return sceneNode; }
    void setSceneNode(SceneNode * node);
    void setPosition(const btVector3 & position);
    btVector3 getPosition() { return worldTransform.getOrigin(); }
    void setTexture(const std::string textureFile);
    void setTotalAnimationTime(float time);
    void setYaw(float yaw);
    float getYaw() { return worldTransform.getRotation().getY(); }
    void setPitch(float pitch);
    float getPitch() { return worldTransform.getRotation().getX(); }
    void setRoll(float roll);
    float getRoll() { return worldTransform.getRotation().getZ(); }
    void addModel(const std::string & colladaFile, float scale, float xOffset, float yOffset, float zOffset, float yaw);
    virtual void getWorldTransform(btTransform & worldTransform) const;
    virtual void setWorldTransform(const btTransform & worldTransform);
    void update(float deltaTime);
    void addControllerByName(std::string name, int controllerId);
    void removeController(int controllerId);
    void setControllerProperty(int controllerId, std::string propertyName, float value);
    void move(float deltaX, float deltaY, float deltaZ);
    void rotate(float deltaAngle, float xAxis, float yAxis, float zAxis);
    void setActor(Actor *theActor) { actor = theActor; }
    Actor *getActor() { return actor; }

private:
    Entity(Area * area, const btVector3 & position, float yaw);
	~Entity();
    btTransform worldTransform;
    float * graphicsWorldMatrix;
    float yaw;
    float pitch;
    float height;
	float scale;
    SceneNode * sceneNode;
    Area * area; 
    Actor *actor;

    std::vector<EntityController *> controllers;

    friend class Area;  // to access destructor
};

#endif
