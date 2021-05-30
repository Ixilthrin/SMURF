#ifndef GENG_SHADER_TEST_1_H
#define GENG_SHADER_TEST_1_H

#include "Model.h"

/**
 *  A basic example of using Cg shaders.
 */
class ShaderTest1 : public Model
{
public:
    ShaderTest1();
    static Model *create();
    void initialize();
	virtual void update(float deltaSeconds);
    virtual void draw(Graphics *graphics, bool picking);
    virtual void acquireReference() {}
    virtual void releaseReference() {}
private:
    bool wasInitialized;
};

#endif
