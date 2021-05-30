#ifndef GENG_SHADER_INFO_H
#define GENG_SHADER_INFO_H

#include <string>

class ShaderInfo
{
public:
    ShaderInfo(int theShaderType, 
               int theMediaType, 
               std::string theFileName, 
               int theVertexType,
               std::string theContextName);
	~ShaderInfo();
    std::string getFileName() { return fileName; }
    int getShaderType() { return shaderType; }
    int getVertexType() { return vertexType; }
    std::string getContextName() { return contextName; }
    enum
    {
        BINARY_FILE,
        TEXT_FILE,
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        SAMPLER_2D,
        MATRIX,
        FLOAT_ARRAY,
        FLOAT_VALUE
    };
private:
    int vertexType;
    int shaderType;
    int mediaType;
    std::string fileName;
    std::string contextName;
};

#endif