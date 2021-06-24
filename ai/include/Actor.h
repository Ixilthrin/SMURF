#ifndef GENG_ACTOR_H
#define GENG_ACTOR_H

#include <aicore/aicore.h>
using namespace aicore;
#include "LinearMath/btVector3.h"
#include <map>
#include <string>

class AIWorldObject;

/**
 *  An object in the AI system that is
 *  controlled by an AIActorController.
 *  The Actor defines the "action state"
 *  of an entity, and the physics system
 *  will determine the actual state of the
 *  entity.
 *
 *  The ai module will not depend on other modules except
 *  math and util.  The interaction between the entity system
 *  and the ai system will be through interfaces, such as
 *  AIWorldObject.
 *
 *  For example, the game will have a set
 *  of entities.  The entities that do something
 *  will each own an Actor.  The Actors will each be assigned
 *  an AIActorController.  A update call will be
 *  made for the Actor, which will call its controller's
 *  update function.  The controller will determine
 *  what the Actor is doing (its velocity, for example)
 *  The physics system will ultimately determine the
 *  entity's actual position in the 3D world based on
 *  what the Actor is doing and the physical environment.
 *  The new position and orientation of the Entity that 
 *  owns the Actor will be fed back into the AI system 
 *  via the AIWorldObject interface.
 */
class Actor
{
public:
    Actor(AIWorldObject *theWorldObject);
    virtual ~Actor();
    void update(float deltaTime);
    AIWorldObject *getWorldObject() { return worldObject; }
    void addBehavior(std::string behaviorName);
    void setCurrentBehavior(std::string behaviorName);
    void setTarget(Actor *theTarget);
    btVector3 getPosition();
    float getYaw();

private:
    aicore::Kinematic *kinematic;
    aicore::SteeringBehaviour *currentBehavior;
    Actor *target;
    AIWorldObject *worldObject;
	std::map<std::string, aicore::SteeringBehaviour *> behaviors;
};

#endif