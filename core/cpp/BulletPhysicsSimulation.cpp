#include "BulletPhysicsSimulation.h"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletAvatar.h"
#include "util.h"

#define ZERO_MASS 0

BulletPhysicsSimulation::BulletPhysicsSimulation(const btVector3 & worldAabbMin, const btVector3 & worldAabbMax, int maxProxies)
: PhysicsSimulation()
{
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
	broadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies); 
    dynamicsWorld = 
		new btDiscreteDynamicsWorld(
			dispatcher,
			broadphase,
			solver,
			collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));

    avatar = new BulletAvatar(dynamicsWorld, broadphase);

}

BulletPhysicsSimulation::~BulletPhysicsSimulation()
{
    delete avatar;
    avatar = 0;

	for (int i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0 ; --i)
	{
		btCollisionObject * obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
	}

    delete dynamicsWorld;
    dynamicsWorld = 0;
	delete broadphase;
    broadphase = 0;
    delete solver;
    solver = 0;
    delete dispatcher;
    dispatcher = 0;
    delete collisionConfiguration;
    collisionConfiguration = 0;
}

void BulletPhysicsSimulation::createSimpleGround()
{
	btCollisionShape * groundShape = 
		new btStaticPlaneShape(btVector3(0, 1, 0), 0);
    collisionShapes.push_back(groundShape);
	btDefaultMotionState* groundMotionState =
		new btDefaultMotionState(
		btTransform(btQuaternion(0, 0, 0, 1), 
		btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody * groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);
}

void BulletPhysicsSimulation::createRigidBody(btCollisionShape * collisionShape,
                         const btTransform & worldTransform,
                         const btTransform & collisionShapeOffset)
{
    btMotionState * motionState = new btDefaultMotionState(worldTransform, collisionShapeOffset); // Deleted by Bullet?
    btRigidBody::btRigidBodyConstructionInfo params(ZERO_MASS, motionState, collisionShape);
    btRigidBody * rigidBody = new btRigidBody(params);  // Deleted when clearing physics world.
    dynamicsWorld->addRigidBody(rigidBody);
}

void BulletPhysicsSimulation::update(float deltaSeconds)
{
    avatar->updatePreSimulation(deltaSeconds);
    dynamicsWorld->stepSimulation(deltaSeconds, 5);
    avatar->updatePostSimulation();
}
