#include "D3DShaderReference.h"
#include "D3DGraphics.h"
#include "dxerr.h"
#include "ShaderInfo.h"
#include "util.h"
#include "TextureReference.h"
#include "GraphicsSystem.h"
#include "VertexBufferInfo.h"

LPDIRECT3DDEVICE9 D3DShaderReference::graphicsDevice(0);

D3DShaderReference::D3DShaderReference(LPDIRECT3DDEVICE9 device, 
                                       int theProgramType, std::string theContextName)
: CgShaderReference(theProgramType), vertexDeclaration(NULL)
{
    if (contextMap.count(theContextName) == 0)
    {
        context = cgCreateContext();
        contextMap[theContextName] = context;
    }
    else
    {
        context = contextMap[theContextName];
    }

    if (device != graphicsDevice)
    {
        graphicsDevice = device;
        cgD3D9SetDevice(graphicsDevice);
    }
}

void D3DShaderReference::createNewContext()
{
    context = cgCreateContext();
    checkForCgError("creating context", context);
}

D3DShaderReference::~D3DShaderReference()
{
}

void D3DShaderReference::load(ShaderInfo *info)
{
    /*
    char *d3d = "-d3d";
    char *prof = "-profile";
    char *dxvs2 = "dxvs2";
    char *po1 = "-po";
    char *numTemps = "NumTemps=14";
    char *po2 = "-po";
    char *numInst = "NumInstructionSlots=300";
    const char **args = (const char**) malloc(8 * sizeof(char*));
    args[0] = d3d;
    args[1] = prof;
    args[2] = dxvs2;
    args[3] = po1;
    args[4] = numTemps;
    args[5] = po2;
    args[6] = numInst;
    args[7] = 0;
    */
    const char **args = 0;

    std::string fileName = info->getFileName();
    char *programString =  getShaderCode(fileName);
    // The profile is based on vertex or fragment processing.
    if (fileName.find("cgvert") != std::string::npos)
    {
        profile = CG_PROFILE_VS_3_0;
		//profile = CG_PROFILE_VP30;
    }
    else if (fileName.find("cgfrag") != std::string::npos)
    {
        profile = CG_PROFILE_PS_3_0;
		//profile = CG_PROFILE_FP30;
    }
    else
    {
        fatalError("Unrecognized shader program file name.");
    }
    program = 0;
    program = cgCreateProgram(context,       // from cgCreateContext
                              CG_SOURCE,     // type: source or object
                              programString, // program text/data
                              (CGprofile) profile, // profile for Direct3D
                              "main",        // entry function name
                              args);         // compiler options

    checkForCgError("creating program", context);
    if (!program)
        fatalError("Cannot create shader program from file.");

    cgD3D9LoadProgram(program,             // CGprogram
                      true,               // Parameter shadowing
                      0);                  // Assembly flags
    checkForCgError("loading program", context);

    if (this->programType == ShaderInfo::VERTEX_SHADER)
    {
        int vertexType = info->getVertexType();
        switch (vertexType)
        {
            case VertexBufferInfo::VERTEX5:
            {
                D3DVERTEXELEMENT9 declaration[] =
                {
                    { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                    { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
                    { 0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                    D3DDECL_END()
                };
                cgD3D9ValidateVertexDeclaration( program, declaration );
                checkForCgError("validating vertex declaration", context);
                graphicsDevice->CreateVertexDeclaration( declaration, &vertexDeclaration );
            }
            break;
            case VertexBufferInfo::VERTEX3:
            case VertexBufferInfo::VERTEX6:
            {
                D3DVERTEXELEMENT9 declaration[] =
                {
                    { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
                    { 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                    D3DDECL_END()
                };
                cgD3D9ValidateVertexDeclaration( program, declaration );
                checkForCgError("validating vertex declaration", context);
                graphicsDevice->CreateVertexDeclaration( declaration, &vertexDeclaration );
            }
            break;
            case VertexBufferInfo::VERTEX7:
            {
                int usage = 1;  // The second set of texture coordinates required a different usage,
                                // but it is not clear why this particular value works.  0 does not work.
                D3DVERTEXELEMENT9 declaration[] =
                {
                    { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                    { 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
                    { 0, 16, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
                    { 0, 28, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
                    { 0, 36, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, usage }, // second set of texture coords.
                    D3DDECL_END()
                };
                cgD3D9ValidateVertexDeclaration( program, declaration );
                checkForCgError("validating vertex declaration", context);
                graphicsDevice->CreateVertexDeclaration( declaration, &vertexDeclaration );
            }
            case VertexBufferInfo::VERTEX8:
            {
                int usage1 = 0;
                int usage2 = 1;  // The second set of texture coordinates required a different usage,
                                // but it is not clear why this particular value works.  0 does not work.
                int usage3 = 2;

                D3DVERTEXELEMENT9 declaration[] =
                {
                    { 0, 0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
                    //{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
                    { 0, 12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
                    { 0, 24, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, usage1 },
                    { 0, 32, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, usage2 }, // second set of texture coords.
                    { 0, 40, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, usage3 },
                    D3DDECL_END()
                };
                cgD3D9ValidateVertexDeclaration( program, declaration );
                checkForCgError("validating vertex declaration", context);
                graphicsDevice->CreateVertexDeclaration( declaration, &vertexDeclaration );
            }
            break;
        }
    }
}

void D3DShaderReference::bindParameterName(std::string name, int type)
{
    parameterTypes[name] = type;
    CGparameter param = cgGetNamedParameter(program, name.c_str());
    checkForCgError("getting named parameter", context);
    parameters[name] = param;
}

void D3DShaderReference::bindAttributeName(std::string name, int type)
{
    bindParameterName(name, type);
}

void D3DShaderReference::setUniformParameter(std::string name, float *value)
{
    CGparameter param = parameters[name];
    cgD3D9SetUniform(param, value);
    checkForCgError("setting uniform parameter", context);
    //parameter = cgGetNamedParameter(program, name.c_str());

}

void D3DShaderReference::setAttribute(std::string name, float *value)
{
    setUniformParameter(name, value);
}

void D3DShaderReference::setUniformTexture(std::string name, TextureReference *texture)
{
    CGparameter param = parameters[name];
    cgD3D9SetTexture(param, (LPDIRECT3DTEXTURE9) (texture->getTextureObject()));
    checkForCgError("setting texture", context);
}

void D3DShaderReference::setUniformArray(std::string name, unsigned int offset, unsigned int numItems, const float *values)
{
    CGparameter param = parameters[name];
    // TODO: This returns an HRESULT so we should handle that (for errors)
    cgD3D9SetUniformArray(param, offset, numItems, values);
    checkForCgError("setting array", context);
}

void D3DShaderReference::setUniformMatrix(std::string name, float *values)
{
    fatalError("D3DShaderReference::setUniformMatrix not implemented.");
}

void D3DShaderReference::activate()
{
    if (this->programType == ShaderInfo::VERTEX_SHADER)
    {
        graphicsDevice->SetVertexDeclaration(vertexDeclaration);
    }
    else if (this->programType == ShaderInfo::FRAGMENT_SHADER)
    {
           // TODO: Find a place to set the texture properties for the shader
           //CGparameter param = parameters["testTexture"];
           // cgD3D9SetSamplerState(param, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
           // cgD3D9SetSamplerState(param, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
           // cgD3D9SetSamplerState(param, D3DSAMP_MINFILTER, D3DTEXF_POINT);
           // cgD3D9SetSamplerState(param, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
           // cgD3D9SetSamplerState(param, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
           // cgD3D9SetTextureWrapMode(param, 0);


    }
    cgD3D9BindProgram(program);
}

void D3DShaderReference::deactivate()
{
    if (this->programType == ShaderInfo::VERTEX_SHADER)
    {
        graphicsDevice->SetVertexShader(0);
    }
    else 
    {
        graphicsDevice->SetPixelShader(0);
    }
}

void D3DShaderReference::destroy()
{
    cgDestroyProgram(program);
}