#ifndef GENG_AI_WORLD_H
#define GENG_AI_WORLD_H

#include "AIWorldObject.h"

#include <vector>

/**
 *  The "world" as the AI system knows it to be.
 *  For now, a simple collection of AIWorldObjects.
 */
class AIWorld
{
public:
    static std::vector<AIWorldObject *> worldObjects;
};

#endif