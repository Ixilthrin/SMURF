// If you need OpenGL extension functions, you only
// need to include this file.  If an extension you
// need is missing, it must be added to this file
// as well as gl_bindings.h and gl_bindings.cpp
#ifndef GENG_GL_BINDINGS_EXTERN_H
#define GENG_GL_BINDINGS_EXTERN_H

extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
extern PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;

#endif
