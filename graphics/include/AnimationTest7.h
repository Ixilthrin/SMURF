#ifndef GENG_ANIMATION_TEST_7_H
#define GENG_ANIMATION_TEST_7_H

#include "Model.h"

/**
 *  Deprecated - this passing bone weights and indices
 *  using uniform variables.  These should be passed
 *  using texture coordinates -  see GeneralModel
 *
 *  Purpose:  Provide support for general models
 *
 *  This class is an example that reads
 *  a model that was exported from Blender
 *  in the .x format and uses Cg shaders
 *  to animate the model.
 * 
 *  It is general in the sense that if the
 *  model does not contain animation and/or
 *  a texture, then the appropriate shader 
 *  will be used.
 */
class AnimationTest7 : public Model
{
public:
    AnimationTest7();
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
