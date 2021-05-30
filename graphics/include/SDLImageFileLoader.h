#ifndef GENG_SDL_IMAGE_FILE_LOADER_H
#define GENG_SDL_IMAGE_FILE_LOADER_H

#include "ImageFileLoader.h"

#include "SDL_image.h"
#include <string>

class SDLImageFileLoader : public ImageFileLoader
{
public:
    SDLImageFileLoader();
    virtual ~SDLImageFileLoader();
    void loadImage(std::string fileName);
private:
    SDL_Surface * surface;
};

#endif