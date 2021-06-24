#ifndef GENG_CONTROLLER_LIBARY_H
#define GENG_CONTROLLER_LIBARY_H

#include "ControllerSample1.h"
#include "EntityController.h"
#include <string>

class Entity;

/**
 *  Add controllers here to make them available to the application.
 */
class ControllerLibrary
{
public:
    static EntityController *createController(Entity *entity, std::string name, int controllerId)
    {
        if (name == std::string("ControllerSample1"))
        {
            return new ControllerSample1(controllerId, entity);
        }
        return 0;
    }
};

#endif
