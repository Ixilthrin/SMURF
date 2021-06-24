#ifndef GENG_CONTROLLER_SAMPLE_1_H
#define GENG_CONTROLLER_SAMPLE_1_H

#include "EntityController.h"

class ControllerSample1 : public EntityController
{
public:
    ControllerSample1(int id, Entity *theEntity);
    virtual void update(float deltaSeconds);
private:
    int xDirection;
};

#endif
