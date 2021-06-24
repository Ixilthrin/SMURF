#include "Area.h"
#include "Avatar.h"
#include "BulletPhysicsSimulation.h"
#include "GraphicsSystem.h"
#include "SceneNode.h"
#include "util.h"
#include "xml_util.h"
#include "utXMLParser.h"
#include "Entity.h"

#define ZERO_MASS 0

std::map<int, btCollisionShape *> entityCollisionShapeIdMap;

Area::Area(std::string thePhysicsName) : physicsName(thePhysicsName)
{
	btVector3 worldAabbMin = btVector3(-1000, -1000, -1000);
    btVector3 worldAabbMax = btVector3(1000, 1000, 1000);
    if (physicsName == std::string("Bullet"))
    {
        BulletPhysicsSimulation *thePhysics = new BulletPhysicsSimulation(worldAabbMin, worldAabbMax, 200);
        thePhysics->createSimpleGround();  // later, do this through a terrain object.
        physicsSimulation = thePhysics;
    }
    else
    {
        physicsSimulation = new PhysicsSimulation();
    }
}

Area::Area(std::string thePhysicsName, const std::string & terrainXml, int maxProxies)
 : physicsName(thePhysicsName)
{
    graphicsSystem = GraphicsSystem::getInstance();

    XMLNode rootNode = parseXmlFile(terrainXml, "terrain");

    // Get extents of area.

    XMLNode extentNode = rootNode.getChildNode("extent");
    if (extentNode.isEmpty())
    {
        fatalError("Extent element missing from " + terrainXml);
    }

    XMLNode minXNode = extentNode.getChildNode("minX");
    if (minXNode.isEmpty())
    {
        fatalError("minX element missing from " + terrainXml);
    }
    float minX = stringToFloat(minXNode.getText());

    XMLNode minYNode = extentNode.getChildNode("minY");
    if (minYNode.isEmpty())
    {
        fatalError("minY element missing from " + terrainXml);
    }
    float minY = stringToFloat(minYNode.getText());

    XMLNode minZNode = extentNode.getChildNode("minZ");
    if (minZNode.isEmpty())
    {
        fatalError("minZ element missing from " + terrainXml);
    }
    float minZ = stringToFloat(minZNode.getText());

    XMLNode maxXNode = extentNode.getChildNode("maxX");
    if (maxXNode.isEmpty())
    {
        fatalError("maxX element missing from " + terrainXml);
    }
    float maxX = stringToFloat(maxXNode.getText());

    XMLNode maxYNode = extentNode.getChildNode("maxY");
    if (maxYNode.isEmpty())
    {
        fatalError("maxY element missing from " + terrainXml);
    }
    float maxY = stringToFloat(maxYNode.getText());

    XMLNode maxZNode = extentNode.getChildNode("maxZ");
    if (maxZNode.isEmpty())
    {
        fatalError("maxZ element missing from " + terrainXml);
    }
    float maxZ = stringToFloat(maxZNode.getText());

    btVector3 worldAabbMin = btVector3(minX, minY, minZ);
    btVector3 worldAabbMax = btVector3(maxX, maxY, maxZ);

    if (physicsName == std::string("Bullet"))
    {
        BulletPhysicsSimulation *thePhysics = new BulletPhysicsSimulation(worldAabbMin, worldAabbMax, 200);
        thePhysics->createSimpleGround();  // later, do this through a terrain object.
        physicsSimulation = thePhysics;
    }
    else
    {
        physicsSimulation = new PhysicsSimulation();
    }
}

Area::~Area()
{
    // Clear physics bodies.
    delete physicsSimulation;
 //   if (dynamicsWorld == 0)
 //   {
 //       return;
 //   }
	//for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; --i)
	//{
	//	btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
	//	btRigidBody * body = btRigidBody::upcast(obj);
	//	if (body && body->getMotionState())
	//	{
	//		delete body->getMotionState();
	//	}
	//	dynamicsWorld->removeCollisionObject(obj);
	//	delete obj;
	//}
    //delete avatar;
    //avatar = 0;
 //   delete dynamicsWorld;
 //   dynamicsWorld = 0;
	//delete broadphase;
 //   broadphase = 0;
 //   delete solver;
 //   solver = 0;
 //   delete dispatcher;
 //   dispatcher = 0;
 //   delete collisionConfiguration;
 //   collisionConfiguration = 0;

    // Clear entities and scene nodes.
	std::vector<Entity *>::iterator it = entities.begin();
	std::vector<Entity *>::iterator end = entities.end();
    for (; it != end; ++it)
	{
        Entity * entity = *it;
        SceneNode * sceneNode = entity->getSceneNode();
        graphicsSystem->destroySceneNode(sceneNode);
		delete entity;
	}
	entities.clear();
    colorIdMap.clear();
}

//void Area::createSimpleGround()
//{
//	btCollisionShape * groundShape = 
//		new btStaticPlaneShape(btVector3(0, 1, 0), 0);
//    collisionShapes.push_back(groundShape);
//	btDefaultMotionState* groundMotionState =
//		new btDefaultMotionState(
//		btTransform(btQuaternion(0, 0, 0, 1), 
//		btVector3(0, 0, 0)));
//	btRigidBody::btRigidBodyConstructionInfo
//		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
//	btRigidBody * groundRigidBody = new btRigidBody(groundRigidBodyCI);
//	dynamicsWorld->addRigidBody(groundRigidBody);
//}

/// Render scale is applied after render offset. 
void Area::createTerrainObject(
                               const btTransform & worldTransform,
                               const std::string & colladaFile, 
                               float renderScale,
                               const btTransform & renderOffset,
                               btCollisionShape * collisionShape,
                               const btTransform & collisionShapeOffset)
{
    // Create scene node.
    GraphicsSystem * graphicsSystem = GraphicsSystem::getInstance();
    SceneNode * sceneNode = graphicsSystem->createSceneNode(colladaFile, "");  // This is deleted when new area loads.
    btTransform renderScaleTransform;
    renderScaleTransform.setIdentity();
    renderScaleTransform.getBasis()[0].setX(renderScale);
    renderScaleTransform.getBasis()[1].setY(renderScale);
    renderScaleTransform.getBasis()[2].setZ(renderScale);
    btTransform graphicsWorldTransform = worldTransform * renderScaleTransform * renderOffset;
    float * graphicsWorldMatrix = sceneNode->getWorldMatrixPtr();
    graphicsWorldTransform.getOpenGLMatrix(graphicsWorldMatrix);

    // Create static rigid body.
    //btTransform physicsWorldTransform = worldTransform * collisionShapeOffset;

    if (physicsName == std::string("Bullet"))
    {
        BulletPhysicsSimulation * physics = (BulletPhysicsSimulation *) physicsSimulation;
        physics->createRigidBody(collisionShape, worldTransform, collisionShapeOffset);
    }

    //btMotionState * motionState = new btDefaultMotionState(worldTransform, collisionShapeOffset); // Deleted by Bullet?
    //btRigidBody::btRigidBodyConstructionInfo params(ZERO_MASS, motionState, collisionShape);
    //btRigidBody * rigidBody = new btRigidBody(params);  // Deleted when clearing physics world.
    //dynamicsWorld->addRigidBody(rigidBody);
}

void Area::createTerrainObject(
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
                             float collisionShapeYawOffset)
{
    btTransform worldTransform(btQuaternion(btVector3(0, 1, 0), DEGS_TO_RADS(yaw)), btVector3(x, y, z));
    btTransform renderOffset(
        btQuaternion(btVector3(0, 1, 0), DEGS_TO_RADS(renderYawOffset)), 
        btVector3(renderXOffset, renderYOffset, renderZOffset));
    if (entityCollisionShapeIdMap.count(collisionShapeId) == 0)
    {
        fatalError("gengEntityAddCollisionShape called with invalid collision shape id");
    }
    btCollisionShape * collisionShape = entityCollisionShapeIdMap[collisionShapeId];
    btTransform collisionShapeOffset(
        btQuaternion(btVector3(0, 1, 0), DEGS_TO_RADS(collisionShapeYawOffset)), 
        btVector3(collisionShapeXOffset, collisionShapeYOffset, collisionShapeZOffset));
    createTerrainObject(
        worldTransform, 
        colladaFile, 
        renderScale, 
        renderOffset, 
        collisionShape, 
        collisionShapeOffset);
}


//void Area::addStaticRigidBody(btRigidBody * rigidBody)
//{
//    dynamicsWorld->addRigidBody(rigidBody);
//}

void Area::update(float deltaSeconds)
{
    std::vector<Entity *>::iterator iter = entities.begin();
    std::vector<Entity *>::iterator end = entities.end();
    for (; iter != end; ++iter)
    {
        (*iter)->update(deltaSeconds);
    }
    physicsSimulation->update(deltaSeconds);
}

// Return pointer to entity picked by mouse click or NULL if no object picked.
Entity * Area::pick(int screenX, int screenY)
{
    unsigned int colorId = graphicsSystem->pick(screenX, screenY);
    if (colorIdMap.count(colorId) == 0)
    {
        return 0;
    }
    else
    {
        return colorIdMap[colorId];
    }
}

Entity * Area::createEntity(const btVector3 & position, float yaw)
{
    Entity * entity = new Entity(this, position, yaw);
	entities.push_back(entity);
	return entity;
}

void Area::createCollisionBox(int id, float xHalfExtent, float yHalfExtent, float zHalfExtent)
{
    entityCollisionShapeIdMap[id] = new btBoxShape(btVector3(xHalfExtent, yHalfExtent, zHalfExtent));
}

void Area::destroyEntity(Entity * entity)
{
    std::vector<Entity *>::iterator iter = entities.begin();
    std::vector<Entity *>::iterator end = entities.end();
    for ( ; iter != end; ++iter)
    {
        if ((*iter) == entity)
        {
            entities.erase(iter);
            break;
        }
    }
    delete entity;
}
