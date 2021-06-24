#include "Actor.h"

#include "AIWorldObject.h"
#include "util.h"

#define WORLD_SIZE 100
#define TRIM_WORLD(var) \
    if (var < -WORLD_SIZE) var = WORLD_SIZE; \
    if (var > WORLD_SIZE) var = -WORLD_SIZE;

Actor::Actor(AIWorldObject *theWorldObject)
 : worldObject(theWorldObject), currentBehavior(0), target(0)
{
    kinematic = new aicore::Kinematic();
    kinematic->position.x = theWorldObject->getPosition().getX();
    kinematic->position.z = theWorldObject->getPosition().getZ();
    kinematic->orientation = theWorldObject->getYaw();
}

Actor::~Actor()
{
    delete kinematic;
    if (currentBehavior)
    {
        delete currentBehavior;
    }
}

void Actor::addBehavior(std::string behaviorName)
{
    if (behaviors.count(behaviorName) > 0)
    {
        // TODO: Warn user that behavior was already added.
        return;
    }
    if (behaviorName == std::string("seek"))
    {
        if (!target)
        {
            fatalError("target not set for seek behavior");
        }
        aicore::Seek *seek = new aicore::Seek();
        seek->character = kinematic;
        seek->target = &(target->kinematic->position);
        seek->maxAcceleration = (aicore::real)20.0;
        behaviors["seek"] = seek;
    }
    else if (behaviorName == std::string("flee"))
    {
        if (!target)
        {
            fatalError("target not set for flee behavior");
        }
        aicore::Flee *flee = new aicore::Flee();
        flee->character = kinematic;
        flee->target = &(target->kinematic->position);
        flee->maxAcceleration = (aicore::real)20.0;
        behaviors["flee"] = flee;
    }
    else if (behaviorName == std::string("wander"))
    {
        aicore::Wander *wander = new aicore::Wander();
        wander->character = kinematic;
        wander->maxAcceleration = (aicore::real)20.0;
        wander->volatility = (aicore::real)40.0;
        wander->turnSpeed = (aicore::real)5.0;
        behaviors["wander"] = wander;
    }
    else
    {
        fatalError(behaviorName + std::string(" is not a recognized behavior"));
    }
}

void Actor::setCurrentBehavior(std::string behaviorName)
{
    if (behaviors.count(behaviorName) == 0)
    {
        fatalError(behaviorName + std::string(" behavior has not been set for the Actor"));
    }
    currentBehavior = behaviors[behaviorName];
}

void Actor::setTarget(Actor *theTarget)
{
    target = theTarget;
}

btVector3 Actor::getPosition()
{
    btVector3 position;
    position.setX(kinematic->position.x);
    position.setY(2.0);
    position.setZ(kinematic->position.z);
    return position;
}

float Actor::getYaw()
{
    return kinematic->orientation;
}

void Actor::update(float deltaTime)
{
    float duration = deltaTime;
    aicore::SteeringOutput steer;


    // Update the steering if we have a movement algorithm
    if (currentBehavior) currentBehavior->getSteering(&steer);
    else steer.clear();

    // Update the kinematic
    kinematic->integrate(steer, (aicore::real)0.95, duration);
    kinematic->setOrientationFromVelocity();

    // Check for maximum speed
    kinematic->trimMaxSpeed((aicore::real)15.0);

    // Keep in bounds of the world
    TRIM_WORLD(kinematic->position.x);
    TRIM_WORLD(kinematic->position.z);
}