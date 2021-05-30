#include "GL_SDL_WindowManager.h"

#include "SDL.h"
#include "opengl_includes.h"
#include "util.h"

void GL_SDL_WindowManager::createWindow()
{
    bool fullScreen = false;
   	if (screenWidth == 0 && screenHeight == 0)
	{
		fullScreen = true;
	}

	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult == -1)
	{
		fatalError("SDL_Init failed: " + std::string(SDL_GetError()));
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_WM_SetCaption(caption.c_str(), NULL);
	Uint32 sdlVideoModeFlags = SDL_OPENGL | SDL_INIT_AUDIO; // remember to remove this after adding FMOD
	if (fullScreen)
	{
		sdlVideoModeFlags |= SDL_FULLSCREEN;
	}
	SDL_Surface * screen = SDL_SetVideoMode(
		screenWidth,
		screenHeight,
		0,
		sdlVideoModeFlags
		);
	if (screen == NULL)
	{
		fatalError("SDL_SetVideoMode failed: " + std::string(SDL_GetError()));
	}
	const SDL_VideoInfo * sdlVideoInfo = SDL_GetVideoInfo();
	screenWidth = sdlVideoInfo->current_w;
	screenHeight = sdlVideoInfo->current_h;

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	SDL_EnableUNICODE(1);  // note: this reduces performance
    SDL_ShowCursor(SDL_ENABLE);
}


void GL_SDL_WindowManager::swapBuffers()
{
   SDL_GL_SwapBuffers();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}