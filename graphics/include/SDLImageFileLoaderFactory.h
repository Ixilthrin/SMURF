#ifndef GENG_SDL_IMAGE_FILE_LOADER_FACTORY_H
#define GENG_SDL_IMAGE_FILE_LOADER_FACTORY_H

#include "ImageFileLoaderFactory.h"
#include "SDLImageFileLoader.h"

class SDLImageFileLoaderFactory : public ImageFileLoaderFactory
{
public:
    virtual ImageFileLoader *createImageFileLoader()
    {
        return new SDLImageFileLoader();
    }
};

#endif