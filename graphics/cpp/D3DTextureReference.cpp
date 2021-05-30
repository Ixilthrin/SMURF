#include "D3DTextureReference.h"

D3DTextureReference::D3DTextureReference(LPDIRECT3DDEVICE9 device)
: graphicsDevice(device)
{
}

D3DTextureReference::~D3DTextureReference()
{
}

void D3DTextureReference::activate()
{
    graphicsDevice->SetTexture(0, texture);
}

void D3DTextureReference::deactivate()
{
	graphicsDevice->SetTexture(0, 0);
}

void D3DTextureReference::destroy()
{
	texture->Release();
}

void D3DTextureReference::writeData(void *data, int dataSize)
{
	//graphicsDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU );
	//graphicsDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE ); 
	//graphicsDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    //graphicsDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
    //graphicsDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	
	if (texture)
	{
	    D3DLOCKED_RECT rect;
	    texture->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
	    memcpy(rect.pBits, data, dataSize);
	    texture->UnlockRect(0);
	}
}


