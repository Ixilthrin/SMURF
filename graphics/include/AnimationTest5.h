#ifndef GENG_ANIMATION_TEST_5_H
#define GENG_ANIMATION_TEST_5_H

#include "Model.h"

/**
 *  This class is a hybrid between animations
 *  that are hard-coded and animations that are
 *  loaded from Blender in the .x format.
 */
class AnimationTest5 : public Model
{
public:
    AnimationTest5();
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