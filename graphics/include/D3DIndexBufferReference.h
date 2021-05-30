#ifndef GENG_D3DINDEXBUFFERREFERENCE_H
#define GENG_D3DINDEXBUFFERREFERENCE_H

#include "IndexBufferReference.h"
#include "IndexBufferInfo.h"
#include <d3d9.h>
#include <d3dx9.h>

class D3DIndexBufferReference : public IndexBufferReference
{
public:
	D3DIndexBufferReference(LPDIRECT3DDEVICE9 device, int numIndices);
	virtual ~D3DIndexBufferReference();
	void setBufferObject(LPDIRECT3DINDEXBUFFER9 bufferObject) { buffer = bufferObject; }
    virtual void activate();
	virtual void deactivate();
    virtual void destroy();
    virtual int getIndexCount() { return indexCount; }
private:
	LPDIRECT3DINDEXBUFFER9 buffer;
	LPDIRECT3DDEVICE9 graphicsDevice;
    int indexCount;
};

#endif
