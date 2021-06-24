#ifndef GENG_PHYSICS_SIMULATION_H
#define GENG_PHYSICS_SIMULATION_H

class Avatar;

class PhysicsSimulation
{
public:
    PhysicsSimulation();
    virtual ~PhysicsSimulation();
    virtual void update(float deltaSeconds);
    Avatar * getAvatar() { return avatar; }

protected:
    Avatar * avatar;

};

#endif
