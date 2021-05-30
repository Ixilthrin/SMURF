#ifndef GENG_GL_BINDINGS
#define GENG_GL_BINDINGS

//#ifndef __APPLE__
//#ifndef WIN32
//#include <glew.h>
//#endif
//#endif

#include "SDL_types.h"
#include "SDL_opengl.h"
#include "SDL.h"
#include "util.h"

// This only needs to be called once during
// program execution.  Currently the graphics
// system makes this call.  If you want to
// add GL extensions, include "gl_bindings_extern.h"
void loadGLBindings();

#endif
