#ifndef GENG_BULLET_AVATAR_H
#define GENG_BULLET_AVATAR_H

#include "Avatar.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"

class Camera;

class BulletAvatar : public Avatar
{
public:
    BulletAvatar(btDiscreteDynamicsWorld * dynamicsWorld, btAxisSweep3 * broadphase);
    virtual ~BulletAvatar();
    virtual void updatePreSimulation(float deltaSeconds);
    virtual void updatePostSimulation();

protected:
    btKinematicCharacterController * character;
    btPairCachingGhostObject * characterGhostObject;
};

#endif
