/*
 *  BMPFileLoader.h
 *  CS_621_Project
 *
 *  Created by David Stover on 5/1/10.
 *  Copyright 2010 David Stover. All rights reserved.
 *
 */

#ifndef __bmp_image_file_loader_h__
#define __bmp_image_file_loader_h__

#include "ImageFileLoader.h"

class BMPImageFileLoader : public ImageFileLoader
{
public:
    BMPImageFileLoader();
    virtual ~BMPImageFileLoader();
    virtual void loadImage(std::string fileName);
};

#endif

