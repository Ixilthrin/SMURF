#include "Gui.h"
#include "GuiImage.h"
#include "GuiTexture.h"
#include "GraphicsSystem.h"
#include "Graphics.h"

GuiImage::GuiImage(
                   int guiOrientation,
                   int offsetX,
                   int offsetY,
                   const std::string & imageFileName)
                   : textureCoordUMax(1.0), textureCoordVMax(1.0)
{
    guiTexture = GuiTexture::load(imageFileName);
    textureCoordUMax = guiTexture->getTextureCoordUMax();
    textureCoordVMax = guiTexture->getTextureCoordVMax();
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    screenCoords = graphics->getAbsoluteScreenCoords(guiOrientation, offsetX, offsetY,
                                                     guiTexture->getWidth(), 
                                                     guiTexture->getHeight());
    Gui::getInstance()->addOverlay(this);
}

GuiImage::~GuiImage()
{
    Gui::getInstance()->removeOverlay(this);
    guiTexture->releaseReference();
}

void GuiImage::update()
{
}

void GuiImage::draw()
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
//    graphics->setColor(color.red, color.green, color.blue, color.alpha);

    if (isSemiTransparent)
    {
        graphics->enableMode(Graphics::ALPHA_TRANSPARENCY);
    }
    graphics->setColor(color);
    guiTexture->getTextureReference()->activate();

    graphics->disableMode(Graphics::CULLING);
    graphics->disableMode(Graphics::LIGHTING);
    indexBuffer->activate();
    vertexBuffer->activate(false);
    graphics->drawIndexedTriangles(6, 0);
    vertexBuffer->deactivate();
    indexBuffer->deactivate();

    guiTexture->getTextureReference()->deactivate();
    
    if (isSemiTransparent)
    {
        graphics->disableMode(Graphics::ALPHA_TRANSPARENCY);
    }
}

bool GuiImage::pick(int x, int y)
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ScreenPosition pos = graphics->getAbsoluteScreenPosition(x, y);
    return graphics->ScreenCoordsContainsPosition(screenCoords, pos);
}
