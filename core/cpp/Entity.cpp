#include "Entity.h"

#include "Area.h"
#include "ControllerLibrary.h"
#include "Drawable.h"
#include "GraphicsSystem.h"
#include "LinearMath/btQuaternion.h"
#include "SceneNode.h"
#include "util.h"
#include "utXMLParser.h"

#include <cmath>
#include <iostream>

Entity::Entity(Area * area, const btVector3 & position, float yaw) : 
sceneNode(0), graphicsWorldMatrix(0), area(area), actor(0), pitch(0), yaw(0), height(0)
{
    worldTransform.setOrigin(position);
    worldTransform.setRotation(btQuaternion(yaw, 0, 0));
    height = position.getY();
}

Entity::~Entity()
{
    GraphicsSystem::getInstance()->destroySceneNode(sceneNode);
}

void Entity::addModel(const std::string & colladaFile, float scale, float xOffset, float yOffset, float zOffset, float yaw)
{
    if (xOffset != 0) fatalError("model offsets not yet supported.");
    GraphicsSystem * graphicsSystem = GraphicsSystem::getInstance();
    sceneNode = graphicsSystem->createSceneNode(colladaFile, "");
    graphicsWorldMatrix = sceneNode->getWorldMatrixPtr();
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
	this->scale = scale;
	sceneNode->setScale(scale);
}

void Entity::getWorldTransform(btTransform & transform) const
{
    transform = worldTransform;
}

void Entity::setWorldTransform(const btTransform & transform)
{
    worldTransform = transform;
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

void Entity::setPosition(const btVector3 & position)
{
    height = position.getY();
    worldTransform.setOrigin(position);
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

void Entity::setTexture(const std::string textureFile)
{
    sceneNode->setTexture(textureFile);
}

void Entity::setTotalAnimationTime(float time)
{
    sceneNode->setTotalAnimationTime(time);
}

void Entity::setYaw(float yaw) 
{ 
    this->yaw = yaw;
	btTransform t;
	t.setRotation(btQuaternion(yaw, 0, 0));
    worldTransform = worldTransform * t;
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

void Entity::setPitch(float pitch) 
{ 
    this->pitch = pitch;
	btTransform t;
	t.setRotation(btQuaternion(0, pitch, 0));
    worldTransform = worldTransform * t;
    //worldTransform.setRotation(btQuaternion(0, pitch, 0));
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

void Entity::setRoll(float roll) 
{ 
    worldTransform.setRotation(btQuaternion(0, roll, 0));
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

void Entity::setPickable(bool pickable)
{
   	if (pickable)
    {
        if (sceneNode->getColorId() != 0)
        {
            // We're already pickable.
            return;
        }
		while (true)
		{
			unsigned char redPicking = (unsigned char) (rand() % 256);
			unsigned char greenPicking = (unsigned char) (rand() % 256);
			unsigned char bluePicking = (unsigned char) (rand() % 256);
        //    unsigned int colorId = rgbToUint(redPicking, greenPicking, bluePicking);
			unsigned int colorId = redPicking | (greenPicking << 8) | (bluePicking << 16);
			if (colorId != 0 && area->colorIdMap.count(colorId) == 0)
			{
                sceneNode->setColorId(colorId);
                area->colorIdMap[colorId] = this;
				return;
			}
		}
	}
	else
	{
        if (sceneNode->getColorId() == 0)
        {
            // We're already not pickable.
            return;
        }
        // use RGBA = 0000 if object can occlude others but can not be picked
		area->colorIdMap.erase(sceneNode->getColorId());
        sceneNode->setColorId(0);
	}
}

static btVector3 axisOfRotation(0.0f, 1.0f, 0.0f);   // y-axis
void Entity::update(float deltaTime)
{
    if (actor)
    {
        actor->update(deltaTime);
        float angle = actor->getYaw();
        btQuaternion rotation;
        //rotation.setRotation(axisOfRotation, angle);
        rotation.setEuler(angle + yaw, pitch, 0);
        worldTransform.setIdentity();
        worldTransform.setRotation(rotation);
        worldTransform.setOrigin(actor->getPosition() + btVector3(0, height, 0));
        worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
    }
    std::vector<EntityController *>::iterator iter = controllers.begin();
    std::vector<EntityController *>::iterator end = controllers.end();
    for (; iter != end; ++iter)
    {
        (*iter)->update(deltaTime);
    }
}

void Entity::addControllerByName(std::string name, int controllerId)
{
    EntityController *controller = ControllerLibrary::createController(this, name, controllerId);
    if (!controller)
    {
        fatalError("Cannot find controller named " + name);
    }
    controllers.push_back(controller);
}

void Entity::removeController(int controllerId)
{
    std::vector<EntityController *>::iterator iter = controllers.begin();
    std::vector<EntityController *>::iterator end = controllers.end();
    for (; iter != end; ++iter)
    {
        if ((*iter)->getId() == controllerId)
        {
            controllers.erase(iter);
            // TODO: delete this or it is a memory leak.
            //delete (*iter);
            return;
        }
    }
    fatalError("Trying to remove controller that does not exist.");
}

void Entity::setControllerProperty(int controllerId, std::string propertyName, float value)
{
    std::vector<EntityController *>::iterator iter = controllers.begin();
    std::vector<EntityController *>::iterator end = controllers.end();
    for (; iter != end; ++iter)
    {
        if ((*iter)->getId() == controllerId)
        {
            (*iter)->setProperty(propertyName, value);
            return;
        }
    }
    fatalError("Trying to set property of controller that does not exist.");
}
    
void Entity::move(float deltaX, float deltaY, float deltaZ)
{
    btVector3 position = worldTransform.getOrigin();
    btVector3 deltaPosition(deltaX, deltaY, deltaZ);
    worldTransform.setOrigin(position + deltaPosition);
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

void Entity::rotate(float deltaAngle, float xAxis, float yAxis, float zAxis)
{
    btVector3 axisOfRotation(xAxis, yAxis, zAxis);
    btTransform rotationTransform;
    rotationTransform.setIdentity();
    btQuaternion rotation;
    rotation.setRotation(axisOfRotation, deltaAngle);
    rotationTransform.setRotation(rotation);
    worldTransform = worldTransform * rotationTransform;
    worldTransform.getOpenGLMatrix(graphicsWorldMatrix);
}

