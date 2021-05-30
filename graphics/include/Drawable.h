#ifndef GENG_DRAWABLE_H
#define GENG_DRAWABLE_H

#include "SelfDeletingResource.h"

struct Color;
class Graphics;
class TextureReference;

class Drawable : public SelfDeletingResource
{
public:
	virtual void update(float deltaSeconds) =0;
    virtual void draw(Graphics *graphics, bool picking) = 0;
    virtual void setTexture(TextureReference *texRef) = 0;
    virtual void setTotalAnimationTime(float time) = 0;
    virtual void setCurrentAnimationTime(float time) {}
    virtual void acquireReference() =0;
    virtual void releaseReference() =0;
	void setWorldMatrix(float *matrix)
	{
		for (int i = 0; i < 16; ++i) {
			worldMatrix[i] = matrix[i];
		}
	}
protected:
    float worldMatrix[16];
};

#endif
