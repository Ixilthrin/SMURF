#include "GLSLShaderReference.h"

#include "opengl_includes.h"

#include "ShaderInfo.h"
#include "util.h"

std::map<std::string, GLuint> GLSLShaderReference::programMap;

GLSLShaderReference::GLSLShaderReference(int theProgramType, std::string theProgramName) 
: ShaderReference(theProgramType)
{
    if (programMap.count(theProgramName) == 0)
    {
        programId = glCreateProgram();
        programMap[theProgramName] = programId;
    }
    else
    {
        programId = programMap[theProgramName];
    }
}

GLSLShaderReference::~GLSLShaderReference()
{
}

void GLSLShaderReference::load(ShaderInfo *info)
{
    
    const char *version;
    
    version = (const char *) glGetString(GL_VERSION);
    if (version[0] != '2' || version[1] != '.') 
    {
        // On Mac, the version fails but the functionality is still supported
#ifndef __APPLE__
        fatalError("This program requires OpenGL 2.x, found " + std::string(version));
#endif
    }
    
    std::string fileName = info->getFileName();
    const GLchar *programString =  getShaderCode(fileName);
    if (fileName.find("glvert") != std::string::npos)
    {
        shaderId = glCreateShader(GL_VERTEX_SHADER);
    }
    else if (fileName.find("glfrag") != std::string::npos)
    {
        shaderId = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
        fatalError(std::string("GLSLShaderReference: Unrecognized shader program file name: ") + fileName);
    }

    GLint shaderCompiled = 1;

    glShaderSource(shaderId, 1, &programString, NULL);
    glCompileShader(shaderId);
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompiled);
    if (!shaderCompiled)
    {
        fatalError("GLSLShaderReference: Shader program not compiled successfully.\n");
    }
    glAttachShader(programId, shaderId);

    if (programType == ShaderInfo::FRAGMENT_SHADER)
    {
        GLint linked;

        glLinkProgram(programId);
        glGetProgramiv(programId, GL_LINK_STATUS, &linked);

        if (!linked)
        {
            fatalError("GLSLShaderReference: Shader program not linked successfully.\n");
        }
    }
}

void GLSLShaderReference::bindParameterName(std::string name, int type)
{
    GLuint param = glGetUniformLocation(programId, name.c_str()); 
    parameters[name] = param;
}

void GLSLShaderReference::bindAttributeName(std::string name, int type)
{
    GLuint param = glGetAttribLocation(programId, name.c_str());
    parameters[name] = param;
}

void GLSLShaderReference::setUniformParameter(std::string name, float *value)
{
    GLuint param = parameters[name];
    glUniform1f(param, *value);
}

void GLSLShaderReference::setAttribute(std::string name, float *value)
{
    GLuint param = parameters[name];
    glVertexAttrib4f(param, *value, 0, 0, 0);
}

void GLSLShaderReference::setUniformTexture(std::string name, TextureReference *texture)
{
}

void GLSLShaderReference::setUniformArray(std::string name, unsigned int offset, unsigned int numItems, const float *values)
{
}

void GLSLShaderReference::setUniformMatrix(std::string name, float *values)
{
}

void GLSLShaderReference::activate()
{
    if (programType == ShaderInfo::VERTEX_SHADER)
    {
        glUseProgram(programId);
    }
}

void GLSLShaderReference::deactivate()
{
    if (programType == ShaderInfo::FRAGMENT_SHADER)
    {
        glUseProgram(0);
    }
}

void GLSLShaderReference::destroy()
{
}