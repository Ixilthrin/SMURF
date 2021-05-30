#include "ShaderInfo.h"

ShaderInfo::ShaderInfo(int theShaderType, 
                       int theMediaType, 
                       std::string theFileName, 
                       int theVertexType,
                       std::string theContextName)
: shaderType(theShaderType), 
  mediaType(theMediaType), 
  fileName(theFileName), 
  vertexType(theVertexType),
  contextName(theContextName)
{
}

ShaderInfo::~ShaderInfo()
{
}
