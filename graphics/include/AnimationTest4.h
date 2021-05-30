#ifndef GENG_ANIMATION_TEST_4_H
#define GENG_ANIMATION_TEST_4_H

#include "Model.h"

class AnimationTest4 : public Model
{
public:
    AnimationTest4();
    static Model *create();
    void initialize();
	virtual void update(float deltaSeconds);
    virtual void draw(Graphics *graphics, bool picking);
    virtual void setCurrentAnimationTime(float time);
    virtual void setTotalAnimationTime(float time);
    virtual void acquireReference() {}
    virtual void releaseReference() {}
private:
    bool wasInitialized;
    float numberOfTransforms;
    float *transforms;  // keyframe transformations
    float currentTime;
    float totalTime; // seconds
};

#endif