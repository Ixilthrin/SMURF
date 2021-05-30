#ifndef GENG_ANIMATION_TEST_2_H
#define GENG_ANIMATION_TEST_2_H

#include "Model.h"

class AnimationTest2 : public Model
{
public:
    AnimationTest2();
    static Model *create();
    void initialize();
	virtual void update(float deltaSeconds);
    virtual void draw(Graphics *graphics, bool picking);
    virtual void acquireReference() {}
    virtual void releaseReference() {}
private:
    bool wasInitialized;
    float *T0;
    float *T1;
    float *T2;
    float *resultT;
    float currentTime;
    float timeStep2;
    float timeStep1;
};

#endif