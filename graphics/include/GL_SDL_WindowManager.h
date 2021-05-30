#ifndef GENG_GL_SDL_WINDOW_MANAGER_H
#define GENG_GL_SDL_WINDOW_MANAGER_H

#include "WindowManager.h"

/**
 *  Concrete class for creating a window using SDL with OpenGL
 */
class GL_SDL_WindowManager : public WindowManager
{
public:
    GL_SDL_WindowManager(std::string windowCaption, int desiredScreenWidth, int desiredScreenHeight, bool useFullscreen)
        :WindowManager(windowCaption, desiredScreenWidth, desiredScreenHeight, useFullscreen)
    {}
    virtual void createWindow();
	virtual void swapBuffers();
};

#endif