#ifndef GENG_D3D_SDL_WINDOW_MANAGER_H
#define GENG_D3D_SDL_WINDOW_MANAGER_H

#include "WindowManager.h"

/**
 *  Concrete class for creating a window using SDL with Direct3D
 */
class D3D_SDL_WindowManager : public WindowManager
{
public:
    D3D_SDL_WindowManager(std::string windowCaption, int desiredScreenWidth, int desiredScreenHeight, bool useFullscreen)
        :WindowManager(windowCaption, desiredScreenWidth, desiredScreenHeight, useFullscreen)
    {}
    virtual void createWindow();
	virtual void swapBuffers();
};

#endif