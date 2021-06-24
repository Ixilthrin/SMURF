#ifndef GENG_AI_WORLD_OBJECT_H
#define GENG_AI_WORLD_OBJECT_H

#include "LinearMath/btVector3.h"

class Actor;

/**
 *  An interface that users of the AI system
 *  will implement for the purpose of adding
 *  objects to the AI world.
 */
class AIWorldObject
{
public:
    virtual Actor *getActor() = 0;
    virtual btVector3 getPosition() = 0;
    virtual float getYaw() = 0;
};

#endif