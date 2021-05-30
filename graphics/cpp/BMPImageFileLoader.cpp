/*
 *  BMPFileLoader.cpp
 *  CS_621_Project
 *
 *  Created by David Stover on 5/1/10.
 *  Copyright 2010 David Stover. All rights reserved.
 *
 */

#include "BMPImageFileLoader.h"
#include "RgbImage.h"


BMPImageFileLoader::BMPImageFileLoader()
{
}

BMPImageFileLoader::~BMPImageFileLoader()
{
}

void BMPImageFileLoader::loadImage(std::string fileName)
{
    RgbImage image(fileName.c_str());
    width = image.GetNumCols();
    height = image.GetNumRows();
    int dataLength = width * height * 3;
    data = new char[dataLength];
    char *imageData = (char *) image.ImageData();
    for (int i = 0; i < dataLength; ++i)
    {
        data[i] = imageData[i];
    }
    //data = (void *) image.ImageData();
    bytesPerPixel = 3;
}
