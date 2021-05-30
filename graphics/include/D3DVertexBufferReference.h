#ifndef GENG_D3DVERTEXBUFFERREFERENCE_H
#define GENG_D3DVERTEXBUFFERREFERENCE_H

#include "VertexBufferReference.h"
#include "VertexBufferInfo.h"
#include <d3d9.h>
#include <d3dx9.h>

class D3DGraphics;

class D3DVertexBufferReference : public VertexBufferReference
{
public:
	D3DVertexBufferReference(D3DGraphics *graphicsObject, int size);
	virtual ~D3DVertexBufferReference();
	void setBufferObject(LPDIRECT3DVERTEXBUFFER9 bufferObject) { buffer = bufferObject; }
	virtual void writeData(void *data, int dataSize);
    virtual void activate(bool picking);
	virtual void deactivate();
    virtual void destroy();
	void setVertexType(int type) { vertexType = type; }
private:
	LPDIRECT3DVERTEXBUFFER9 buffer;
	int vertexSize;
	int vertexType; // same as VertexBufferInfo vertexType
    D3DGraphics *graphics;
};

#endif
