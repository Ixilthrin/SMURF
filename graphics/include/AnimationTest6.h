#ifndef GENG_ANIMATION_TEST_6_H
#define GENG_ANIMATION_TEST_6_H

#include "Model.h"

/**
 *  This class is an example that reads
 *  a model that was exported from Blender
 *  in the .x format and uses Cg shaders
 *  to animate the model.
 */
class AnimationTest6 : public Model
{
public:
    AnimationTest6();
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
    int numberOfKeyframes;
    int numberOfBones;
    float *transforms;  // keyframe transformations - to be removed
    float *frameTransforms; // Each bone has a single frame transform
    float *keyframeTransforms; // Each bone has a transform for each keyframe
    float *offsetTransforms;  // Each bone has a single offset transform
    float *skinWeights; // Maps vertices to bone weights
    float currentTime;
    float totalTime; // seconds
};

#endif
