#ifdef WIN32

#include "D3DIndexBufferReference.h"


D3DIndexBufferReference::D3DIndexBufferReference(LPDIRECT3DDEVICE9 device, int numIndices)
: buffer(NULL), graphicsDevice(device), indexCount(numIndices)
{
}

D3DIndexBufferReference::~D3DIndexBufferReference()
{
}

void D3DIndexBufferReference::activate()
{
	graphicsDevice->SetIndices(buffer);
}

void D3DIndexBufferReference::deactivate()
{
	graphicsDevice->SetIndices(0);
}

void D3DIndexBufferReference::destroy()
{
}

#endif

