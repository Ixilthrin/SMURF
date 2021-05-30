#include "SDLImageFileLoader.h"

#include "util.h"

SDLImageFileLoader::SDLImageFileLoader()
: ImageFileLoader(), surface(0)
{
}

SDLImageFileLoader::~SDLImageFileLoader()
{
    if (surface)
    {
        SDL_FreeSurface(surface);
    }
}

void SDLImageFileLoader::loadImage(std::string fileName)
{
    assertFileExists(fileName);
    SDL_Surface * surface = IMG_Load(fileName.c_str());
    if (surface == 0)
    {
        fatalError("Failed to load file: " + fileName + " " + IMG_GetError());
    }
    SDL_PixelFormat * format = surface->format;
    if (format->BytesPerPixel == 3)
    {
        bytesPerPixel = 3;
    }
    else if (format->BytesPerPixel == 4)
    {
        bytesPerPixel = 4;
    }
    else
    {
        fatalError("Unsupported bytes per pixel in image " + fileName);
    }
    width = surface->w;
    height = surface->h;
    data = (char *) surface->pixels;
}
