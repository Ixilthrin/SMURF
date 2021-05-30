#ifndef GENG_GENERAL_MODEL_H
#define GENG_GENERAL_MODEL_H

#include "Model.h"

/**
 *  Purpose:  Provide support for general models.
 *    This means the model can be static or have
 *    animation.
 *
 *  This model supports keyframe animation where
 *  the tweening (bone interpolation) is performed
 *  in the CPU, and the skinning (vertex interpolation)
 *  is performed in a vertex shader.  Previous
 *  tests have performed tweening and skinning in
 *  the vertex shader.  This has two problems:
 *    (1) This seriously limits the number of bones used.
 *    (2) The tweening is performed for each vertex.
 *
 *  This class is an example that reads
 *  a model that was exported from Blender
 *  in the .x format and uses Cg shaders
 *  to animate the model.
 * 
 *  It is general in the sense that if the
 *  model does not contain animation and/or
 *  a texture, then the appropriate shader 
 *  will be used. It is also last in the series
 *  of incremental tests to experiment with
 *  keyframe animations.  See AnimationTest1
 *  through AnimationTest8.
 */

class GeneralModel : public Model
{
public:
    GeneralModel();
    static Model *create(std::string modelName);
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
    float *boneTransforms;  // Current set of bone transforms to be passed to the shader
    float *skinWeights; // Maps vertices to bone weights
    float currentTime;
    float totalTime; // seconds
};

#endif
