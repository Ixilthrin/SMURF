#include "ControllerSample1.h"

#include "LinearMath/btTransform.h"
#include "Entity.h"
#include "math.h"
#include "util.h"

ControllerSample1::ControllerSample1(int id, Entity *theEntity)
: EntityController(id, theEntity), xDirection(1)
{
}

void ControllerSample1::update(float deltaSeconds)
{
    btTransform transform;
    entity->getWorldTransform(transform);
    btVector3 position = transform.getOrigin();
    if (position.getX() > 30 && xDirection == 1)
    {
        xDirection = -1;     
    }
    else if (position.getX() < -30 && xDirection == -1)
    {
        xDirection = 1;
    }

    entity->rotate(.05f, 0.0f, 1.0f, 0.0f);
    entity->move(.1f * (float) xDirection, 0, 0);
}
