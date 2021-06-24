#include "BulletAvatar.h"
#include "Area.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "util.h"

BulletAvatar::BulletAvatar(btDiscreteDynamicsWorld * dynamicsWorld, btAxisSweep3 * broadphase)
: Avatar()
{
    camera = Camera::getInstance();

    // Create the kinematic character controller that collides with static rigid bodies.
    btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 0.0, 10.0));  // character is moved onto ground regardless of y
    startTransform.setRotation(btQuaternion(DEGS_TO_RADS(180), 0, 0)); // face into negative z

	characterGhostObject = new btPairCachingGhostObject();
	characterGhostObject->setWorldTransform(startTransform);
	broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

 	btScalar characterHeight = 1.75;
	btScalar characterWidth = 0.50;
	btConvexShape * capsule = new btCapsuleShape(characterWidth, characterHeight);
	characterGhostObject->setCollisionShape(capsule);
	characterGhostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
    // TODO: Figure out why a non-zero step height causes
    // intermittent jumping when running in DirectX mode.
    // The original value of stepHeight was .35
	btScalar stepHeight = btScalar(0.0f);
	character = new btKinematicCharacterController(characterGhostObject, capsule, stepHeight);

    // Only collide with static objects.
	dynamicsWorld->addCollisionObject(
        characterGhostObject, 
        btBroadphaseProxy::CharacterFilter, 
        btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	dynamicsWorld->addAction(character);
}

BulletAvatar::~BulletAvatar()
{
    delete character;
    //delete characterGhostObject; // I believe this is deleted by Bullet.
}

void BulletAvatar::updatePreSimulation(float deltaSeconds)
{
    btTransform xform = characterGhostObject->getWorldTransform();

    // Synchronize rotation with camera.
    //btQuaternion rotation(DEGS_TO_RADS(camera->getYaw() + 180), DEGS_TO_RADS(camera->getPitch()), 0);
    //xform.setRotation(rotation);

//    btMatrix3x3 orn = xform.getBasis();
//    orn = btMatrix3x3(btQuaternion(btVector3(0,1,0), DEGS_TO_RADS(camera->getYaw())));
//    xform.setBasis(orn);

    // Set the walkDirection for the character.
    btVector3 forwardDir = xform.getBasis()[2];
    btVector3 upDir = xform.getBasis()[1];
    btVector3 strafeDir = xform.getBasis()[0];
    forwardDir.normalize();
    upDir.normalize();
    strafeDir.normalize();

	// Walk velocity magic number hidden here apparently making it difficult to find and adjust
    btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
    btScalar walkVelocity = btScalar(1.1) * 12.0; // 4 km/h -> 1.1 m/s
    btScalar walkSpeed = walkVelocity * deltaSeconds;

    if (moveLeftOnUpdate)
    {
        walkDirection += strafeDir;
        moveLeftOnUpdate = false;
    }
    if (moveRightOnUpdate)
    {
        walkDirection -= strafeDir;
        moveRightOnUpdate = false;
    }
    if (moveForwardOnUpdate)
    {
        walkDirection += forwardDir;
        moveForwardOnUpdate = false;
    }
    if (moveBackwardOnUpdate)
    {
        walkDirection -= forwardDir;
        moveBackwardOnUpdate = false;
    }

    // Set the orientation
    btVector3 axisOfRotation(0.0f, 1.0f, 0.0f);   // y-axis
    btQuaternion rotation;
    float currentAngle = DEGS_TO_RADS(camera->getYaw());
    rotation.setRotation(axisOfRotation, currentAngle);
    btTransform transform;
    transform.setIdentity();
    transform.setRotation(rotation);
    walkDirection = transform * walkDirection;

    character->setWalkDirection(walkDirection * walkSpeed);
}

void BulletAvatar::updatePostSimulation()
{
    btTransform xform = characterGhostObject->getWorldTransform();
    btVector3 cameraPosition = xform.getOrigin();
    camera->setPosition(cameraPosition.getX(), cameraPosition.getY(), cameraPosition.getZ());
}
