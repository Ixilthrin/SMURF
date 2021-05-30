// Uncomment one of the following defines
// or define one of the following when compiling
//#define USING_SDL_WITH_OPENGL
//#define USING_GLUT_WITH_GLEW
//#define USING_OPENGL_WITH_IPHONE
//#define USING_GLUT_WITH_COCOA

#ifndef __OPENGL_INCLUDES_H__
#define __OPENGL_INCLUDES_H__

#ifdef USING_SDL_WITH_OPENGL
#include "SDL_opengl.h"
#include "gl_bindings.h"
#ifndef GL_GLEXT_PROTOTYPES  // defined in SDL_opengl.h
#include "gl_bindings_extern.h"
#endif
#endif

#ifdef USING_GLUT_WITH_GLEW
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#endif

#ifdef USING_OPENGL_WITH_IPHONE
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#endif

#ifdef USING_GLUT_WITH_COCOA
#include "GLUT/glut.h"
#endif

#endif
