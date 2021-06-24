#include <sstream>
#include "SDLEventDispatcher.h"
#include "SDL.h"
#include "Color.h"
#include "util.h"


SDLEventDispatcher::SDLEventDispatcher() : 
    EventDispatcher()
{
}

void SDLEventDispatcher::init() 
{ 
    camera = Camera::getInstance();
}

SDLEventDispatcher::~SDLEventDispatcher()
{
}

void SDLEventDispatcher::toggleMouseVisible()
{
    if (mouseVisible)
	{
   		SDL_ShowCursor(SDL_DISABLE);
        mouseVisible = false;
	}
	else
	{
   		SDL_ShowCursor(SDL_ENABLE);
        mouseVisible = true;
	}
}

void SDLEventDispatcher::processEvents()
{
    static bool dropNextMouseMotionEvent = false; // Used to keep mouse in center of screen when not visible.
	SDLKey key;
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch(sdlEvent.type)
		{
		case SDL_QUIT:
            if (windowClosingHandler != 0)
            {
                // Don't call SDLQuit; instead, wait for call to shutdown().
                windowClosingHandler();
            }
            else
            {
                SDL_Quit();
            }
			break;
		case SDL_MOUSEMOTION:
        {
            int dx = sdlEvent.motion.xrel;
            int dy = -sdlEvent.motion.yrel;
            if (dropNextMouseMotionEvent)
            {
                dropNextMouseMotionEvent = false;
            }
            else if (mouseVisible)
            {
                if (mouseMotionHandler != 0)
                {
                    mouseMotionHandler(dx, dy);
                }
            }
            else // mouse is not visible
            {
			    if (mouseMotionHandler != 0) 
                {
                    mouseMotionHandler(dx, dy);
                }
                dropNextMouseMotionEvent = true; // to avoid processing subsequent warp_mouse call
                SDL_WarpMouse(camera->getScreenWidth() / 2, camera->getScreenHeight() / 2);  // generates mouse motion event
            }
			break;
        }
		case SDL_MOUSEBUTTONDOWN:
        {
       		int x = sdlEvent.button.x;
		    int y = camera->getScreenHeight() - sdlEvent.button.y;
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
			{
                if (leftMouseButtonDownHandler != 0)
                {
				    leftMouseButtonDownHandler(x, y);
                }
			}
			else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
			{
                if (rightMouseButtonDownHandler != 0)
                {
				    rightMouseButtonDownHandler(x, y);
                }
			}
            else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE)
			{
                if (middleMouseButtonDownHandler != 0)
                {
				    middleMouseButtonDownHandler(x, y);
                }
			}
            // When user rolls mouse wheel, SDL generates 2 events: 
            // SDL_MOUSEBUTTONDOWN followed by SDL_MOUSEBUTTONUP so we only
            // process SDL_MOUSEBUTTONDOWN and ignore SDL_MOUSEBUTTONUP.
            else if (sdlEvent.button.button == SDL_BUTTON_WHEELDOWN)
			{
                if (mouseWheelRollDownHandler != 0)
                {
				    mouseWheelRollDownHandler();
                }
			}
            else if (sdlEvent.button.button == SDL_BUTTON_WHEELUP)
			{
                if (mouseWheelRollUpHandler != 0)
                {
				    mouseWheelRollUpHandler();
                }
			}
			break;
        }
        case SDL_MOUSEBUTTONUP:
        {
       		int x = sdlEvent.button.x;
		    int y = camera->getScreenHeight() - sdlEvent.button.y;
			if (sdlEvent.button.button == SDL_BUTTON_LEFT)
			{
                if (leftMouseButtonUpHandler != 0)
                {
				    leftMouseButtonUpHandler(x, y);
                }
			}
			else if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
			{
                if (rightMouseButtonUpHandler != 0)
                {
				    rightMouseButtonUpHandler(x, y);
                }
			}
            else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE)
			{
                if (middleMouseButtonUpHandler != 0)
                {
				    middleMouseButtonUpHandler(x, y);
                }
			}
			break;
        }
        case SDL_KEYDOWN:
			key = sdlEvent.key.keysym.sym;
            if (keyDownHandler != 0)
            {
                keyDownHandler(key);
            }
			break;
		case SDL_KEYUP:
			key = sdlEvent.key.keysym.sym;
            if (keyUpHandler != 0)
            {
                keyUpHandler(key);
            }
			break;
		}
	}
}
