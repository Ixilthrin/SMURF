#ifndef GENG_TERRAIN_SAMPLE_1_H
#define GENG_TERRAIN_SAMPLE_1_H

#include "Model.h"

class TerrainSample1 : public Model
{
public:
    TerrainSample1();
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
    float *resultT;
    float currentTime;
    float totalTime; // seconds
};

#endif