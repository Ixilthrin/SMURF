#ifndef GENG_ANIMATION_TEST_3_H
#define GENG_ANIMATION_TEST_3_H

#include "Model.h"

class AnimationTest3 : public Model
{
public:
    AnimationTest3();
    static Model *create();
    void initialize();
	virtual void update(float deltaSeconds);
    virtual void draw(Graphics *graphics, bool picking);
    virtual void acquireReference() {}
    virtual void releaseReference() {}
private:
    bool wasInitialized;
    int numberOfTransforms;
    float **transforms;  // keyframe transformations
    float *resultTransformation;
    float currentTime;
    float totalTime; // seconds
};

#endif