#ifndef GENG_GUI_IMAGE_CREATOR_H
#define GENG_GUI_IMAGE_CREATOR_H

#include "GuiImageInfo.h"
#include "GuiImage.h"

class GuiImageCreator
{
public:
    virtual GuiImage *createGuiImage(GuiImageInfo *info) = 0;
};

#endif