#include "EntityController.h"

#include "Entity.h"

EntityController::EntityController(int theId, Entity *theEntity)
: id(theId), entity(theEntity)
{
}

/**
 *  Override this method to control the entity.
 *  If you don't override this method, then your
 *  controller won't do anything.
 */
void EntityController::update(float deltaSeconds)
{
}

/**
 *  Override this method to set values for this controller.
 *  You can add fields and then set their values from the application.
 *  For example, you can create a field representing the
 *  entity id for an enemy entity, and then set the value using
 *  this method.
 */
void EntityController::setProperty(std::string propertyName, float value)
{
}