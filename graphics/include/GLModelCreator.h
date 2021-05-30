#ifndef GENG_GL_MODEL_CREATOR_H
#define GENG_GL_MODEL_CREATOR_H

#include "ModelCreator.h"

/**
 *   Used to create a Model for OpenGL graphics.
 */
class GLModelCreator : public ModelCreator
{
public:
    GLModelCreator();
    virtual Model *createModel(ModelInfo *info);
private:
    Model *createTestTriangle();
};

#endif
