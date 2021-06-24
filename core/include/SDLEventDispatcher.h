#ifndef GENG_SDL_EVENT_DISPATCHER_H
#define GENG_SDL_EVENT_DISPATCHER_H

#include "EventDispatcher.h"

#include <string>

struct Color;

class SDLEventDispatcher : public EventDispatcher
{
public:
    SDLEventDispatcher();
    virtual ~SDLEventDispatcher();
    virtual void init();
    virtual void processEvents();
    virtual void toggleMouseVisible();  // TODO: Move to different class
};

#endif
