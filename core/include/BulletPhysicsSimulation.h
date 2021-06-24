#ifndef GENG_BULLET_PHYSICS_SIMULATION_H
#define GENG_BULLET_PHYSICS_SIMULATION_H

#include "PhysicsSimulation.h"

#include "btBulletDynamicsCommon.h"

class BulletPhysicsSimulation : public PhysicsSimulation
{
public:
    BulletPhysicsSimulation(const btVector3 & worldAabbMin, const btVector3 & worldAabbMax, int maxProxies);
    virtual ~BulletPhysicsSimulation();
    void createSimpleGround();
    void createRigidBody(btCollisionShape * collisionShape,
                         const btTransform & worldTransform,
                         const btTransform & collisionShapeOffset);
    virtual void update(float deltaSeconds);

protected:
	btVector3 worldAabbMin;
    btVector3 worldAabbMax;
	btAxisSweep3 * broadphase;
	btDefaultCollisionConfiguration * collisionConfiguration;
	btCollisionDispatcher * dispatcher;
	btSequentialImpulseConstraintSolver * solver;
	btDiscreteDynamicsWorld * dynamicsWorld;
    btAlignedObjectArray<btCollisionShape *> collisionShapes;
};

#endif
