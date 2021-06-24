#ifndef GENG_EVENT_DISPATCHER_H
#define GENG_EVENT_DISPATCHER_H

#include <string>
#include "Camera.h"

struct Color;

class EventDispatcher
{
public:
    typedef void (* WindowClosingHandler)();
    typedef void (* MouseMotionHandler)(int dx, int dy);
    typedef void (* LeftMouseButtonDownHandler)(int dx, int dy);
    typedef void (* RightMouseButtonDownHandler)(int dx, int dy);
    typedef void (* MiddleMouseButtonDownHandler)(int dx, int dy);
    typedef void (* LeftMouseButtonUpHandler)(int dx, int dy);
    typedef void (* RightMouseButtonUpHandler)(int dx, int dy);
    typedef void (* MiddleMouseButtonUpHandler)(int dx, int dy);
    typedef void (* MouseWheelRollDownHandler)();
    typedef void (* MouseWheelRollUpHandler)();
    typedef void (* KeyDownHandler)(int key);
    typedef void (* KeyUpHandler)(int key);

    //static EventDispatcher * getInstance();
    virtual ~EventDispatcher();
    virtual void init() = 0;
    virtual void toggleMouseVisible() = 0;  // TODO: Move to different class
    virtual void processEvents() = 0;

    // Event callback functions
    void setWindowClosingHandler(WindowClosingHandler windowClosingHandler) { this->windowClosingHandler = windowClosingHandler; }
    void setMouseMotionHandler(MouseMotionHandler mouseMotionHandler) { this->mouseMotionHandler = mouseMotionHandler; }
    void setLeftMouseButtonDownHandler(LeftMouseButtonDownHandler leftMouseButtonDownHandler) { this->leftMouseButtonDownHandler = leftMouseButtonDownHandler; }
    void setRightMouseButtonDownHandler(RightMouseButtonDownHandler rightMouseButtonDownHandler) { this->rightMouseButtonDownHandler = rightMouseButtonDownHandler; }
    void setMiddleMouseButtonDownHandler(MiddleMouseButtonDownHandler middleMouseButtonDownHandler) { this->middleMouseButtonDownHandler = middleMouseButtonDownHandler; }
    void setLeftMouseButtonUpHandler(LeftMouseButtonUpHandler leftMouseButtonUpHandler) { this->leftMouseButtonUpHandler = leftMouseButtonUpHandler; }
    void setRightMouseButtonUpHandler(RightMouseButtonUpHandler rightMouseButtonUpHandler) { this->rightMouseButtonUpHandler = rightMouseButtonUpHandler; }
    void setMiddleMouseButtonUpHandler(MiddleMouseButtonUpHandler middleMouseButtonUpHandler) { this->middleMouseButtonUpHandler = middleMouseButtonUpHandler; }
    void setMouseWheelRollDownHandler(MouseWheelRollDownHandler mouseWheelRollDownHandler) { this->mouseWheelRollDownHandler = mouseWheelRollDownHandler; }
    void setMouseWheelRollUpHandler(MouseWheelRollUpHandler mouseWheelRollUpHandler) { this->mouseWheelRollUpHandler = mouseWheelRollUpHandler; }
    void setKeyDownHandler(KeyDownHandler keyDownHandler) { this->keyDownHandler = keyDownHandler; }
    void setKeyUpHandler(KeyUpHandler keyUpHandler) { this->keyUpHandler = keyUpHandler; }

protected:
    EventDispatcher();

    //void setInstance(EventDispatcher *theInstance) { instance = theInstance; }
    static EventDispatcher *instance;

    Camera * camera;
    bool mouseVisible;

    WindowClosingHandler windowClosingHandler;
    MouseMotionHandler mouseMotionHandler;
    LeftMouseButtonDownHandler leftMouseButtonDownHandler;
    RightMouseButtonDownHandler rightMouseButtonDownHandler;
    MiddleMouseButtonDownHandler middleMouseButtonDownHandler;
    LeftMouseButtonUpHandler leftMouseButtonUpHandler;
    RightMouseButtonUpHandler rightMouseButtonUpHandler;
    MiddleMouseButtonUpHandler middleMouseButtonUpHandler;
    MouseWheelRollDownHandler mouseWheelRollDownHandler;
    MouseWheelRollUpHandler mouseWheelRollUpHandler;
    KeyDownHandler keyDownHandler;
    KeyUpHandler keyUpHandler;
};

#endif
