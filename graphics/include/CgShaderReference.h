#ifndef GENG_CG_SHADER_REFERENCE_H
#define GENG_CG_SHADER_REFERENCE_H

#include "ShaderReference.h"

#include "Cg/cg.h"
#include <string>
#include <map>

class ShaderInfo;
class TextureReference;

class CgShaderReference : public ShaderReference
{
public:
    CgShaderReference(int theProgramType);
    virtual ~CgShaderReference() {};

    void checkForCgError(const char *situation, CGcontext context);
protected:
    static std::map<std::string, CGcontext> contextMap;

    CGcontext context;
    CGprofile profile;
    CGprogram program;
    std::map<std::string, CGparameter>  parameters; // TODO: should we use CgParamter* instead of CgParameter?
};

#endif
