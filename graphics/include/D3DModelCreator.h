#ifndef GENG_D3D_MODEL_CREATOR_H
#define GENG_D3D_MODEL_CREATOR_H

#include "ModelCreator.h"

/**
 *   Used to create a Model for Direct3D graphics.
 */
class D3DModelCreator : public ModelCreator
{
public:
    D3DModelCreator();
    virtual Model *createModel(ModelInfo *info);
};

#endif
