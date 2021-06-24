#include "Avatar.h"
#include "Area.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "util.h"

Avatar::Avatar()
{
    camera = Camera::getInstance();
}

Avatar::~Avatar()
{
}

void Avatar::moveForward()
{
    moveForwardOnUpdate = true;
}

void Avatar::moveBackward()
{
    moveBackwardOnUpdate = true;
}

void Avatar::strafeLeft()
{
    moveLeftOnUpdate = true;
}

void Avatar::strafeRight()
{
    moveRightOnUpdate = true;
}

void Avatar::updatePreSimulation(float deltaSeconds)
{
}

void Avatar::updatePostSimulation()
{
}
