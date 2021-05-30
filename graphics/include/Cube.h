#ifndef GENG_CUBE_H
#define GENG_CUBE_H

// TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"

#include "Drawable.h"
//#include "Color.h"

struct Color;
class Graphics;

/**
Users of this class should call delete on instance to release resource.
*/
class Cube : public Drawable
{
public:
	static Cube * create();
    virtual void acquireReference();
    virtual void releaseReference();
    virtual void setTexture(TextureReference *texRef) {}
    virtual void setTotalAnimationTime(float time) {}
	virtual void update(float deltaSeconds);
    virtual void draw(Graphics *graphics, bool picking);

private:
	static int totalReferenceCount;
    static GLuint displayListName;
	int referenceCount;
	Cube();
    virtual ~Cube() { }
};

#endif
