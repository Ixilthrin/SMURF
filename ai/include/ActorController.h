#ifndef GENG_AI_ACTOR_CONTROLLER_H
#define GENG_AI_ACTOR_CONTROLLER_H

class Actor;

/**
 *  NOT CURRENTLY USED
 *
 *  An abstract class.  A subclass will
 *  function as the "brain" of an Actor
 */
class AIActorController
{
public:
    AIActorController(Actor *theActor) : actor(theActor) {}
    virtual ~AIActorController() {}
    virtual void update(float deltaTime) = 0;
private:
    Actor *actor;
};

#endif