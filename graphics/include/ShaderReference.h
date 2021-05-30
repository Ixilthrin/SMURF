#ifndef GENG_SHADER_REFERENCE_H
#define GENG_SHADER_REFERENCE_H

#include <string>
#include <map>

class ShaderInfo;
class TextureReference;

class ShaderReference
{
public:
    ShaderReference(int theProgramType);
    virtual ~ShaderReference() {};
    virtual void load(ShaderInfo *info) = 0;
    virtual void bindParameterName(std::string name, int type) = 0;
    virtual void bindAttributeName(std::string name, int type) = 0;
    virtual void setUniformParameter(std::string name, float *value) = 0;
    virtual void setUniformTexture(std::string name, TextureReference *texture) = 0;
    virtual void setUniformArray(std::string name, unsigned int offset, unsigned int numItems, const float *values) = 0;
    virtual void setUniformMatrix(std::string name, float *values) = 0;
    virtual void setAttribute(std::string name, float *value) = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void destroy() = 0;

protected:

    int programType; //ShaderInfo::VERTEX_SHADER or ShaderInfo::FRAGMENT_SHADER
    char *getShaderCode(std::string fileName);
    std::map<std::string, int> parameterTypes;  // map<name, type> : type is ShaderInfo::MATRIX, ShaderInfo::Sampler_2D, etc
};

#endif
