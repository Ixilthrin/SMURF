// Additional OpenGL bindings should be added
// to this file, gl_bindings.h, and gl_bindings_extern.h
#include "gl_bindings.h"

#ifdef WIN32
//#ifndef __APPLE__
PFNGLBINDBUFFERPROC glBindBuffer = 0;
PFNGLGENBUFFERSPROC glGenBuffers = 0;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;
PFNGLBUFFERDATAPROC glBufferData = 0;
PFNGLMAPBUFFERPROC glMapBuffer = 0;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = 0;
PFNGLBUFFERSUBDATAPROC glBufferSubData = 0;
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = 0;
PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture = 0;
PFNGLCREATESHADERPROC glCreateShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLSHADERSOURCEPROC glShaderSource = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLGETSHADERIVPROC glGetShaderiv = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLUNIFORM1FPROC glUniform1f = 0;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f = 0;
#endif

static bool bindingsLoaded = false;

void loadGLBindings()
{
	if (bindingsLoaded)
		return;
	bindingsLoaded = true;
	
#ifdef WIN32
//#ifndef __APPLE__
    glBindBuffer = (PFNGLBINDBUFFERPROC) SDL_GL_GetProcAddress("glBindBuffer");
    if (glBindBuffer == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed.  glBindBuffer not found");
    }
    glGenBuffers = (PFNGLGENBUFFERSPROC) SDL_GL_GetProcAddress("glGenBuffers");
    if (glGenBuffers == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed.  glGenBuffers not found");
    }
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) SDL_GL_GetProcAddress("glDeleteBuffers");
    if (glDeleteBuffers == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed.  glDeleteBuffers not found");
    }
    glBufferData = (PFNGLBUFFERDATAPROC) SDL_GL_GetProcAddress("glBufferData");
    if (glBufferData == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glBufferData not found");
    }
    glMapBuffer = (PFNGLMAPBUFFERPROC) SDL_GL_GetProcAddress("glMapBuffer");
    if (glMapBuffer == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed.  glMapBuffer not found");
    }
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) SDL_GL_GetProcAddress("glUnmapBuffer");
    if (glUnmapBuffer == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glUnmapBuffer not found");
    }
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC) SDL_GL_GetProcAddress("glBufferSubData");
    if (glUnmapBuffer == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glBufferSubData not found.");
    }
    glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) SDL_GL_GetProcAddress("glDrawRangeElements");
    if (glDrawRangeElements == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glDrawRangeElements not found.");
    }
    glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) SDL_GL_GetProcAddress("glClientActiveTexture");
    if (glClientActiveTexture == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glClientActiveTexture not found.");
    }
    glCreateShader = (PFNGLCREATESHADERPROC) SDL_GL_GetProcAddress("glCreateShader");
    if (glCreateShader == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glClientActiveTexture not found.");
    }
    glCreateProgram = (PFNGLCREATEPROGRAMPROC) SDL_GL_GetProcAddress("glCreateProgram");
    if (glCreateProgram == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glCreateProgram not found.");
    }
    glShaderSource = (PFNGLSHADERSOURCEPROC) SDL_GL_GetProcAddress("glShaderSource");
    if (glShaderSource == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glShaderSource not found.");
    }
    glCompileShader = (PFNGLCOMPILESHADERPROC) SDL_GL_GetProcAddress("glCompileShader");
    if (glCompileShader == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glCompileShader not found.");
    }
    glGetShaderiv = (PFNGLGETSHADERIVPROC) SDL_GL_GetProcAddress("glGetShaderiv");
    if (glGetShaderiv == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glGetShaderiv not found.");
    }
    glAttachShader = (PFNGLATTACHSHADERPROC) SDL_GL_GetProcAddress("glAttachShader");
    if (glAttachShader == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glAttachShader not found.");
    }
    glLinkProgram = (PFNGLLINKPROGRAMPROC) SDL_GL_GetProcAddress("glLinkProgram");
    if (glLinkProgram == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glLinkProgram not found.");
    }
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC) SDL_GL_GetProcAddress("glGetProgramiv");
    if (glGetProgramiv == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glGetProgramiv not found.");
    }
    glUseProgram = (PFNGLUSEPROGRAMPROC) SDL_GL_GetProcAddress("glUseProgram");
    if (glUseProgram == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glUseProgram not found.");
    }
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) SDL_GL_GetProcAddress("glGetUniformLocation");
    if (glGetUniformLocation == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glGetUniformLocation not found.");
    }
    glUniform1f = (PFNGLUNIFORM1FPROC) SDL_GL_GetProcAddress("glUniform1f");
    if (glUniform1f == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glUniform1f not found.");
    }
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) SDL_GL_GetProcAddress("glGetAttribLocation");
    if (glGetAttribLocation == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glGetAttribLocation not found.");
    }
    glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC) SDL_GL_GetProcAddress("glVertexAttrib4f");
    if (glVertexAttrib4f == 0)
    {
        fatalError("SDL_GL_GetProcAddress failed. glVertexAttrib4f not found.");
    }
#endif
}
