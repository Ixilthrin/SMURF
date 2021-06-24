#include <sstream>
#include "EventDispatcher.h"
#include "Color.h"
#include "util.h"
#include "Camera.h"

EventDispatcher::EventDispatcher() : 
    mouseVisible(true),
    windowClosingHandler(0),
    mouseMotionHandler(0),
    leftMouseButtonDownHandler(0),
    rightMouseButtonDownHandler(0),
    leftMouseButtonUpHandler(0),
    rightMouseButtonUpHandler(0),
    keyDownHandler(0),
    keyUpHandler(0),
    camera(0)
{
}

EventDispatcher::~EventDispatcher()
{
}

