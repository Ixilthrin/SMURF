#ifndef GENG_WINDOW_MANAGER_H
#define GENG_WINDOW_MANAGER_H

#include <string>

/**
 *  Abstract base class for creating a window
 */
class WindowManager
{
public:
    WindowManager(std::string windowCaption, int desiredScreenWidth, int desiredScreenHeight, bool useFullscreen)
        : caption(windowCaption), screenWidth(desiredScreenWidth), screenHeight(desiredScreenHeight), fullScreen(useFullscreen)
    {
    }
    virtual void createWindow() = 0;
	virtual void swapBuffers() = 0;
    int getScreenWidth() { return screenWidth; }
    int getScreenHeight() { return screenHeight; }
protected:
    std::string caption;
    int screenWidth;
    int screenHeight;
    bool fullScreen;
};

#endif