#include "GuiImageInfo.h"

GuiImageInfo::GuiImageInfo(int guiOrientation, int offsetX, int offsetY, const char * guiImageFileName)
: imageFileName(guiImageFileName), orientation(guiOrientation)
{
    this->offsetX = offsetX;
    this->offsetY = offsetY;
}

GuiImageInfo::~GuiImageInfo()
{
}