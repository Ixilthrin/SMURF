#include "GuiOverlay.h"
#include "GuiTexture.h"
#include "Camera.h"
#include "Gui.h"


GuiOverlay::~GuiOverlay()
{
}

void GuiOverlay::setVisible(bool visible)
{
    Gui * gui = Gui::getInstance();
    if (visible && !this->visible)
    {
        gui->addOverlay(this);
        this->visible = true;
    }
    else if (!visible && this->visible)
    {
        gui->removeOverlay(this);
        this->visible = false;
    }
}

bool GuiOverlay::pick(int x, int y)
{
    return x >= screenCoords.left && x <= screenCoords.right 
        && y >= screenCoords.bottom && y <= screenCoords.top;
}

void GuiOverlay::bringToFront()
{
}
