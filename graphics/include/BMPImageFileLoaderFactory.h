/*
 *  BMPImageFileLoaderFactory.h
 *  CS_621_Project
 *
 *  Created by David Stover on 5/1/10.
 *  Copyright 2010 David Stover. All rights reserved.
 *
 */
#ifndef __BMP_IMAGE_FILE_LOADER_FACTORY_H__
#define __BMP_IMAGE_FILE_LOADER_FACTORY_H__

#include "ImageFileLoaderFactory.h"
#include "BMPImageFileLoader.h"

class BMPImageFileLoaderFactory : public ImageFileLoaderFactory
{
public:
    virtual ImageFileLoader *createImageFileLoader()
    {
        return new BMPImageFileLoader();
    }
};

#endif
