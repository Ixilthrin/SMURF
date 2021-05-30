#ifndef GENG_D3D_SHADER_REFERENCE_H
#define GENG_D3D_SHADER_REFERENCE_H

#include "CgShaderReference.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <Cg/cgD3D9.h>

class ShaderInfo;

class D3DShaderReference : public CgShaderReference
{
public:
    D3DShaderReference(LPDIRECT3DDEVICE9 device, int theProgramType, std::string theContextName);
	virtual ~D3DShaderReference();
    virtual void load(ShaderInfo *info);
    virtual void bindParameterName(std::string name, int type);
    virtual void bindAttributeName(std::string name, int type);
    virtual void setUniformParameter(std::string name, float *value);
    virtual void setUniformTexture(std::string name, TextureReference *texture);
    virtual void setUniformArray(std::string name, unsigned int offset, unsigned int numItems, const float *values);
    virtual void setUniformMatrix(std::string name, float *values);
    virtual void setAttribute(std::string name, float *value);
    virtual void activate();
	virtual void deactivate();
    virtual void destroy();
    CGcontext getContext() { return context; }
    void setContext(CGcontext theContext) { context = theContext; }
    void createNewContext();
private:
    static LPDIRECT3DDEVICE9 graphicsDevice;
    LPDIRECT3DVERTEXDECLARATION9 vertexDeclaration;  // Only applies to vertex shader. TODO: should this be refactored?
};

#endif