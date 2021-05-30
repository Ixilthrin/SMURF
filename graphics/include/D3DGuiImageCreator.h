#ifndef GENG_D3D_GUI_IMAGE_CREATOR_H
#define GENG_D3D_GUI_IMAGE_CREATOR_H

#include "GuiImageCreator.h"

class D3DGuiImageCreator : public GuiImageCreator
{
public:
    virtual GuiImage *createGuiImage(GuiImageInfo *info);
};

#endif