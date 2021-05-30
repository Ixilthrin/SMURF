#ifndef GENG_GL_GUI_IMAGE_CREATOR_H
#define GENG_GL_GUI_IMAGE_CREATOR_H

#include "GuiImageCreator.h"

class GLGuiImageCreator : public GuiImageCreator
{
public:
    virtual GuiImage *createGuiImage(GuiImageInfo *info);
};

#endif