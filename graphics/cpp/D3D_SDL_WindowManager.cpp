#include "D3D_SDL_WindowManager.h"

#include "D3DGraphics.h"
#include "GraphicsSystem.h"
#include "SDL.h"
#include "util.h"

void D3D_SDL_WindowManager::createWindow()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult == -1)
	{
		fatalError("SDL_Init failed: " + std::string(SDL_GetError()));
	}
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_WM_SetCaption(caption.c_str(), NULL);
	//Uint32 sdlVideoModeFlags = SDL_OPENGL;
	Uint32 sdlVideoModeFlags = 0  | SDL_INIT_AUDIO;  // remember to remove this after adding FMOD
	if (fullScreen)
	{
		sdlVideoModeFlags = SDL_FULLSCREEN | SDL_DOUBLEBUF;
	}
	else
	{
	    sdlVideoModeFlags = SDL_RESIZABLE;
	}
	SDL_Surface * screen = SDL_SetVideoMode(
		screenWidth,
		screenHeight,
		SDL_GetVideoInfo()->vfmt->BitsPerPixel,
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

void D3D_SDL_WindowManager::swapBuffers()
{
    // Delegate this function to the Graphics object since the
    // graphics object owns the Direct3D graphics device.
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    dynamic_cast<D3DGraphics*> (graphics)->swapBuffers();
}