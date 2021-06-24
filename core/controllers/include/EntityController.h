#ifndef GENG_ENTITY_NODE_CONTROLLER_H
#define GENG_ENTITY_NODE_CONTROLLER_H

#include <string>

class Entity;

class EntityController
{
public:
    EntityController(int id, Entity *theEntity);
    virtual void update(float deltaSeconds);
    virtual void setProperty(std::string propertyName, float value);
    int getId() { return id; }
protected:
    int id;  // Unique identifier for this controller
    Entity *entity;
};

#endif
