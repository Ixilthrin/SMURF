#include "GLShaderReference.h"
#include "GLGraphics.h"
#include "ShaderInfo.h"
#include <Cg/cgGL.h>
#include "util.h"

GLShaderReference::GLShaderReference(int theProgramType, std::string theContextName)
: CgShaderReference(theProgramType)
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
}

GLShaderReference::~GLShaderReference()
{
}

void GLShaderReference::load(ShaderInfo *info)
{
    cgSetParameterSettingMode(context, CG_DEFERRED_PARAMETER_SETTING);
    checkForCgError("defer parameter setting", context);

    const char **args = 0;

    std::string fileName = info->getFileName();
    char *programString =  getShaderCode(fileName);
    // The profile is based on vertex or fragment processing.
    if (fileName.find("cgvert") != std::string::npos)
    {
        profile = cgGLGetLatestProfile(CG_GL_VERTEX);
        cgGLSetOptimalOptions(profile);
        checkForCgError("selecting vertex profile", context);
    }
    else if (fileName.find("cgfrag") != std::string::npos)
    {
        profile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
        cgGLSetOptimalOptions(profile);
        checkForCgError("selecting fragment profile", context);
    }
    else
    {
        fatalError("Unrecognized shader program file name.");
    }
    program = 0;
    program = cgCreateProgram(context,       // from cgCreateContext
                              CG_SOURCE,     // type: source or object
                              programString, // program text/data
                              (CGprofile) profile, // cg compiler profile
                              "main",        // entry function name
                              args);         // compiler options
    
    checkForCgError("created program", context);

    if (!program)
    {
        fatalError("Cannot create shader program from file.");
    }

    cgGLLoadProgram(program);
    checkForCgError("loading program", context);
}

void GLShaderReference::bindParameterName(std::string name, int type)
{
    parameterTypes[name] = type;
    CGparameter param = cgGetNamedParameter(program, name.c_str());
    parameters[name] = param;
    checkForCgError("getting parameter", context);
}

void GLShaderReference::bindAttributeName(std::string name, int type)
{
    bindParameterName(name, type);
}

void GLShaderReference::setUniformParameter(std::string name, float *value)
{
    CGparameter param = parameters[name];
    cgSetMatrixParameterfr(param, value);
    //cgUpdateProgramParameters(program);
    
    checkForCgError("setting parameter", context);
}

void GLShaderReference::setAttribute(std::string name, float *value)
{
    setUniformParameter(name, value);
}
    
void GLShaderReference::setUniformArray(std::string name, unsigned int offset, unsigned int numItems, const float *values)
{
    CGparameter param = parameters[name];
    cgGLSetParameterArray4f(param, offset, numItems, values);
    //cgUpdateProgramParameters(program);
    checkForCgError("Cg error setting array in GLShaderReference", context);
}

void GLShaderReference::setUniformMatrix(std::string name, float *values)
{
    CGparameter param = parameters[name];
    cgSetMatrixParameterfr(param, values);
    //cgUpdateProgramParameters(program);
    
    checkForCgError("setting parameter", context);
}

void GLShaderReference::activate()
{
    checkForCgError("activating shader", context);
    cgGLBindProgram(program);
    checkForCgError("bind program", context);
    cgGLEnableProfile(profile);
    checkForCgError("enabling profile", context);
}

void GLShaderReference::deactivate()
{
    cgGLUnbindProgram(profile);
    cgGLDisableProfile(profile);
    checkForCgError("deactivating shader", context);
}

void GLShaderReference::destroy()
{
    cgDestroyProgram(program);
}