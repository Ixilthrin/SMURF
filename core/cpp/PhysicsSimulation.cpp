#include "PhysicsSimulation.h"

#include "Avatar.h"
#include "util.h"

#define ZERO_MASS 0

PhysicsSimulation::PhysicsSimulation()
{
    avatar = new Avatar();
}

PhysicsSimulation::~PhysicsSimulation()
{
    delete avatar;
    avatar = 0;
}

void PhysicsSimulation::update(float deltaSeconds)
{
    avatar->updatePreSimulation(deltaSeconds);
    avatar->updatePostSimulation();
}
