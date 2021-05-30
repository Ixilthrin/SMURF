#ifndef GENG_MODEL_CREATOR_H
#define GENG_MODEL_CREATOR_H

#include "ModelInfo.h"
#include "Model.h"

/**
 *  Abstract class. Concrete implementations will create
 *  a model for the appropriate graphics environment.
 */
class ModelCreator
{
public:
    virtual Model *createModel(ModelInfo *info) = 0;
};

#endif

