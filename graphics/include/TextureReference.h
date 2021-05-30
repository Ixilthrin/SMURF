#ifndef GENG_TEXTURE_REFERENCE_H
#define GENG_TEXTURE_REFERENCE_H

class TextureReference
{
public:
	virtual ~TextureReference() {};
	virtual void writeData(void *data, int dataSize)= 0;
    virtual void activate() = 0;
	virtual void deactivate() = 0;
    virtual void destroy() = 0;
    virtual void *getTextureObject() = 0;
};

#endif