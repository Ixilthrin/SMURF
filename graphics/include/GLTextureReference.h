#ifndef GENG_GLTEXTURE_REFERENCE_H
#define GENG_GLTEXTURE_REFERENCE_H

#include "opengl_includes.h"
#include "TextureReference.h"

class GLTextureReference : public TextureReference
{
public:
	GLTextureReference(int theWidth, int theHeight, int thePixelFormat, bool dynamic);
	virtual ~GLTextureReference();
    virtual void activate();
	virtual void deactivate();
    virtual void destroy();
    virtual void *getTextureObject() { return &textureId; }
	virtual void writeData(void *data, int dataSize);
    void setTextureId(GLuint id) { textureId = id; }
    void setHeight(int h) { height = h; }
    void setWidth(int w) { width = w; }
    void setPixelFormat(int f) { pixelFormat = f; }
private:
    GLuint textureId;
	int width;
	int height;
	int pixelFormat;  // e.g., set to TextureReference::ARGB32_FORMAT
	bool isDynamic;
};

#endif