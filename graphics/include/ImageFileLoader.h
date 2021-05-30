#ifndef GENG_IMAGE_FILE_LOADER_H
#define GENG_IMAGE_FILE_LOADER_H

#include <string>

class ImageFileLoader
{
public:
    ImageFileLoader();
    virtual ~ImageFileLoader();
    virtual void loadImage(std::string fileName) = 0;
    int getWidth() { return width; }
    int getHeight() { return height; }
    int getBytesPerPixel() { return bytesPerPixel; }
    void *getData() { return data; }
protected:
    int width;
    int height;
    int bytesPerPixel;
    char *data;
};

#endif