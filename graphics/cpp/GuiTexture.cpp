#include "util.h"
#include "GuiTexture.h"
#include "ImageFileLoader.h"
#include "TextureInfo.h"
#include "Graphics.h"
#include "GraphicsSystem.h"

std::map<std::string, GuiTexture *> GuiTexture::textureMap;

GuiTexture * GuiTexture::load(const std::string & fileName)
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    GuiTexture * guiTexture = 0;
    if (textureMap.count(fileName) > 0)
    {
        guiTexture = textureMap[fileName];
    }
    else
    {
        guiTexture = new GuiTexture(); 
        guiTexture->fileName = fileName;
        ImageFileLoaderFactory *imageLoaderFactory = GraphicsSystem::getInstance()->getImageFileLoaderFactory();
        ImageFileLoader *imageLoader = imageLoaderFactory->createImageFileLoader();
        imageLoader->loadImage(fileName);
        guiTexture->width = imageLoader->getWidth();
        guiTexture->height = imageLoader->getHeight();
        int pixelFormat = 0;
        int dataSize = 0;
        if (imageLoader->getBytesPerPixel() == 3)
        {
            pixelFormat = TextureInfo::RGB24_FORMAT;
            dataSize = imageLoader->getWidth() * imageLoader->getHeight() * 3;
        }
        else if (imageLoader->getBytesPerPixel() == 4)
        {
            pixelFormat = TextureInfo::ARGB32_FORMAT;
            dataSize = imageLoader->getWidth() * imageLoader->getHeight() * 4;
        }
        TextureInfo *textureInfo = new TextureInfo(false, pixelFormat);
        textureInfo->setGenerateMipmap(false);

        // For Direct3D, we need to add padding to make sure the image
        // width is a power of 2.
        if (graphics->usingDirect3D())
        {
            int currentWidth = imageLoader->getWidth();
            int currentHeight = imageLoader->getHeight();

            int newWidth = currentWidth;  // Will be set to a power of 2
            int newHeight = currentHeight; // Will be set to a power of 2

            unsigned int powerOfTwo = 1;
            while (powerOfTwo < (unsigned int) newWidth)
            {
                powerOfTwo = powerOfTwo << 1;
            }
            newWidth = powerOfTwo;

            powerOfTwo = 1;
            while (powerOfTwo < (unsigned int) newHeight)
            {
                powerOfTwo = powerOfTwo << 1;
            }
            newHeight = powerOfTwo;

            guiTexture->textureCoordUMax = (float) currentWidth / (float) newWidth;
            guiTexture->textureCoordVMax = (float) currentHeight / (float) newHeight;

            int horizontalPadding = (newWidth - currentWidth) * imageLoader->getBytesPerPixel();
            
            int newDataSize = newWidth * newHeight * imageLoader->getBytesPerPixel();

            char * pb = new char[newDataSize];
            int rowSize = imageLoader->getWidth() * imageLoader->getBytesPerPixel();
            
            int newPosition = 0;
            for (int y = 0; y < currentHeight; y++) 
            {
                char * pc =  reinterpret_cast<char *>(((char*)imageLoader->getData()) + y * rowSize);
                int oldPosition = 0;
                //char * p = pb + y * newRowSize;	 
                for (int i = 0; i < imageLoader->getWidth(); ++i) 
                {
                    pb[newPosition] = pc[oldPosition+2];   // blue
                    pb[newPosition+1] = pc[oldPosition+1];  // green
                    pb[newPosition+2] = pc[oldPosition]; // red
                    if (imageLoader->getBytesPerPixel() == 3)
                    {
                        oldPosition += 3;
                        newPosition += 3;
                    }
                    else // PIX_FMT_RGB32
                    {
                        pb[newPosition+3] = 0; // alpha
                        oldPosition += 4;
                        newPosition += 4;
                    }
                }
                newPosition += horizontalPadding;
            }
            textureInfo->setHeight(newHeight);
            textureInfo->setWidth(newWidth);
            //textureInfo->setPixelData(imageLoader.getData());
            guiTexture->textureRef = graphics->createTexture(textureInfo);
            guiTexture->textureRef->writeData(pb, newDataSize);
            delete pb;
        
            //guiTexture->textureRef = graphics->createTexture(textureInfo);
            //guiTexture->textureRef->writeData(imageLoader.getData(), dataSize);
        }


        if (!graphics->usingDirect3D())
        {
            textureInfo->setHeight(imageLoader->getHeight());
            textureInfo->setWidth(imageLoader->getWidth());
            textureInfo->setPixelData(imageLoader->getData());
            guiTexture->textureRef = graphics->createTexture(textureInfo);
        }

        textureMap[fileName] = guiTexture;
        delete textureInfo;
        delete imageLoader;
    }
    guiTexture->acquireReference();
    return guiTexture;
}

void GuiTexture::acquireReference()
{
    ++referenceCount;
}

void GuiTexture::releaseReference()
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
