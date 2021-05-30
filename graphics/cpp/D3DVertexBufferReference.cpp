#ifdef WIN32

#include "D3DVertexBufferReference.h"
#include "D3DGraphics.h"
#include "dxerr.h"


D3DVertexBufferReference::D3DVertexBufferReference(D3DGraphics *graphicsObject, int size)
: buffer(NULL), graphics(graphicsObject), vertexSize(size)
{
}

D3DVertexBufferReference::~D3DVertexBufferReference()
{
}

void D3DVertexBufferReference::writeData(void *data, int dataSize)
{
	void *vertexBufferMemory;

	// Write the vertex data into memory referenced by the vertex buffer.
	HRESULT hr = buffer->Lock(0, 0,(void**)&vertexBufferMemory, 0);
    if (FAILED(hr))
    { 
        MessageBox(GetActiveWindow(),L"Failed to lock the vertex buffer", DXGetErrorDescription(hr), MB_OK); 
        return;
    }
	memcpy(vertexBufferMemory, data, dataSize);
    buffer->Unlock();
}

void D3DVertexBufferReference::activate(bool picking)
{
    LPDIRECT3DDEVICE9 device = graphics->getDevice();
    if (graphics->getShaderIsActive())
    {
        device->SetFVF(NULL);
    }
    else
    {
        // Here the flexible vertex format (FVF) is set to match the vertex data.
        switch (vertexType)
        {
        case VertexBufferInfo::VERTEX1:
            device->SetFVF((D3DFVF_XYZ | D3DFVF_TEX1));
            break;
        case VertexBufferInfo::VERTEX2:
            break;
        case VertexBufferInfo::VERTEX3:
            if (picking)
                device->SetFVF((D3DFVF_XYZ));
            else
                device->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE));
            break;
        case VertexBufferInfo::VERTEX4:
            device->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
            break;
        case VertexBufferInfo::VERTEX5:
            device->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1);
            break;
        case VertexBufferInfo::VERTEX6:
            if (picking)
                device->SetFVF((D3DFVF_XYZ));
            else
                device->SetFVF((D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1));
            break;
        case VertexBufferInfo::VERTEX7:
            // TODO: Vertex7 has normals and 2 sets of texture coordinates.  They should be used.
            device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
            //device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX2);
            break;
        case VertexBufferInfo::VERTEX8:
            // TODO: Vertex8 has normals and 3 sets of texture coordinates.  They should be used.
            device->SetFVF(D3DFVF_XYZ);
            //device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_NORMAL | D3DFVF_TEX2);
            break;
        }
    }
    if (picking)
    {
        D3DMATERIAL9 material;
        D3DXCOLOR color(D3DGraphics::getInstance()->getCurrentColor());
        D3DCOLORVALUE colorValue;
        colorValue.r = color.r;
        colorValue.g = color.g;
        colorValue.b = color.b;
        colorValue.a = color.a;
        material.Emissive = color;
        device->SetMaterial(&material);
        device->SetRenderState(D3DRS_LIGHTING, TRUE);
    }
    else
    {
        device->SetRenderState(D3DRS_LIGHTING, FALSE);
    }
    device->SetStreamSource(0, buffer, 0, vertexSize);
}

void D3DVertexBufferReference::deactivate()
{
    LPDIRECT3DDEVICE9 device = graphics->getDevice();
	device->SetStreamSource(0, 0, 0, 0);
}

void D3DVertexBufferReference::destroy()
{
}

#endif

