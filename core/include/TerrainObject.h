#ifndef GENG_TERRAIN_OBJECT_H
#define GENG_TERRAIN_OBJECT_H

#include <btBulletDynamicsCommon.h>

class TerrainObject : public btMotionState
{
public:
    virtual void getWorldTransform(btTransform & worldTransform) const;
    virtual void setWorldTransform(const btTransform & worldTransform);

private:

};

#endif
