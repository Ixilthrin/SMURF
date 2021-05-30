#ifndef GENG_IMAGE_FILE_LOADER_FACTORY_H
#define GENG_IMAGE_FILE_LOADER_FACTORY_H

class ImageFileLoader;

class ImageFileLoaderFactory
{
public:
    virtual ImageFileLoader *createImageFileLoader() = 0;
};

#endif