#ifndef GENG_GLSL_SHADER_REFERENCE_H
#define GENG_GLSL_SHADER_REFERENCE_H

#include "ShaderReference.h"

#include "opengl_includes.h"
#include <map>
#include <string>

class ShaderInfo;
class TextureReference;

class GLSLShaderReference : public ShaderReference
{
public:
    GLSLShaderReference(int theProgramType, std::string theProgramName);
    virtual ~GLSLShaderReference();
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
protected:
    static std::map<std::string, GLuint> programMap;
    GLuint shaderId;
    GLuint programId;
    std::map<std::string, GLuint>  parameters;
};

#endif
