#include "util.h"
#include "SceneTexture.h"
#include "ImageFileLoader.h"
#include "ImageFileLoaderFactory.h"
#include "TextureInfo.h"
#include "Graphics.h"
#include "GraphicsSystem.h"

#include "BMPImageFileLoader.h"

std::map<std::string, SceneTexture *> SceneTexture::textureMap;

/**
 *  Represents a texture that uses mipmapping.
 *  This should be refactored with GuiTexture
 */
SceneTexture * SceneTexture::load(const std::string & fileName)
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    SceneTexture * sceneTexture = 0;
    if (textureMap.count(fileName) > 0)
    {
        sceneTexture = textureMap[fileName];
    }
    else
    {
        sceneTexture = new SceneTexture(); 
        sceneTexture->fileName = fileName;
        ImageFileLoaderFactory *imageLoaderFactory = GraphicsSystem::getInstance()->getImageFileLoaderFactory();
        ImageFileLoader *imageLoader = imageLoaderFactory->createImageFileLoader();
        if (fileName == std::string("images/test.bmp"))
        {
            imageLoader = new BMPImageFileLoader();
        }
        imageLoader->loadImage(fileName);
        sceneTexture->width = imageLoader->getWidth();
        sceneTexture->height = imageLoader->getHeight();
        int pixelFormat = 0;
        int dataSize = 0;
        if (imageLoader->getBytesPerPixel() == 3)
        {
            dataSize = imageLoader->getWidth() * imageLoader->getHeight() * 3;
            pixelFormat = TextureInfo::RGB24_FORMAT;
        }
        else if (imageLoader->getBytesPerPixel() == 4)
        {
            dataSize = imageLoader->getWidth() * imageLoader->getHeight() * 4;
            pixelFormat = TextureInfo::ARGB32_FORMAT;
        }
        if (graphics->usingDirect3D())
        {
            // The image will be converted to 32 bit depth
            pixelFormat = TextureInfo::ARGB32_FORMAT;
        }
        // Implement mipmapping and set TextureInfo to use mipmapping.
        TextureInfo *textureInfo = new TextureInfo(false, pixelFormat);
        textureInfo->setGenerateMipmap(false);
        textureInfo->setHeight(imageLoader->getHeight());
        textureInfo->setWidth(imageLoader->getWidth());
        if (!graphics->usingDirect3D())
        {
            textureInfo->setPixelData(imageLoader->getData());
        }
        sceneTexture->textureRef = graphics->createTexture(textureInfo);

        // TODO: Refactor this with GuiTexture.cpp
        // TODO: Add padding to make height and width both powers of 2.
        // TODO: Should actually be powers of 4?
        if (graphics->usingDirect3D())
        {
            // The image will be converted to 32 bit depth
            int newDataSize = 4 * imageLoader->getWidth() * imageLoader->getHeight();
            char * pb = new char[newDataSize];
            int size = imageLoader->getWidth() * imageLoader->getBytesPerPixel();
            int newsize = imageLoader->getWidth() * 4;
            for (int y = 0; y < imageLoader->getHeight(); y++) 
            {
                char * pc =  reinterpret_cast<char *>(((char*)imageLoader->getData()) + y * size);
                int k = 0;
                int j = 0;
                char * p = pb + y * newsize;	
                int width = imageLoader->getWidth();
                int height = imageLoader->getHeight();
                for (int i = 0; i < imageLoader->getWidth(); ++i) 
                {
                    p[j] = pc[k];   // red
                    p[j+1] = pc[k+1];  // green
                    p[j+2] = pc[k+2]; // blue
                    if (imageLoader->getBytesPerPixel() == 3)
                    {
                        k += 3;
                    }
                    else // PIX_FMT_RGB32
                    {
                        k += 4;
                    }
                    p[j+3] = 0; // alpha
                    j += 4;
                }
            }


            sceneTexture->textureRef->writeData(pb, newDataSize);
            delete pb;
        }
        textureMap[fileName] = sceneTexture;
        delete textureInfo;
        delete imageLoader;
    }
    sceneTexture->acquireReference();
    return sceneTexture;
}

void SceneTexture::acquireReference()
{
    ++referenceCount;
}

void SceneTexture::releaseReference()
{
    --referenceCount;
    if (referenceCount == 0)
    {
        textureMap.erase(fileName);

        delete this;
        return;
    }
    else if (referenceCount < 0)
    {
        fatalError("GuiTexture::releaseReference() called more times than acquireReference.");
    }
}
