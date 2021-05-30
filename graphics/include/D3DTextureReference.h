#ifndef GENG_D3D_TEXTURE_REFERENCE_H
#define GENG_D3D_TEXTURE_REFERENCE_H

#include "TextureReference.h"
#include <d3d9.h>
#include <d3dx9.h>

class D3DTextureReference : public TextureReference
{
public:
	D3DTextureReference(LPDIRECT3DDEVICE9 device);
	virtual ~D3DTextureReference();
	void setTextureObject(LPDIRECT3DTEXTURE9 textureObject) { texture = textureObject; }
	void writeData(void *data, int dataSize);
    virtual void activate();
	virtual void deactivate();
    virtual void destroy();
    virtual void *getTextureObject() { return texture; }
private:
	LPDIRECT3DDEVICE9 graphicsDevice;
    LPDIRECT3DTEXTURE9 texture;
};

#endif
