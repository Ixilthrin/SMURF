#include "Gui.h"
#include "GuiText.h"
#include "GraphicsSystem.h"
#include "Graphics.h"

GuiText::GuiText(int guiOrientation, int offsetX, int offsetY, GuiFont * font, const std::string & textString)
: font(font), textHasChanged(true), fontGeometry(0)
{
    int width = 0;
    int height = 0;
    font->computeDimensions(textString, width, height);
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    screenCoords = graphics->getAbsoluteScreenCoords(guiOrientation, offsetX, 
                                                     offsetY, width, height);
    setTextString(textString);
    Gui::getInstance()->addOverlay(this);
}

GuiText::~GuiText()
{
    Gui::getInstance()->removeOverlay(this);
}

void GuiText::setTextString(const std::string & textString)
{
    textHasChanged = true;
    this->textString = textString;
}

void GuiText::update()
{
    if (textHasChanged || positionHasChanged)
    {
        // re-generate text here
        // TODO: delete fontGeometry before creating a new one.
        fontGeometry = font->updateTextAndPosition(textString, screenCoords.left, screenCoords.top);
        textHasChanged = false;
        positionHasChanged = false;
    }
}

void GuiText::draw()
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();  // TODO: should pass graphics as arg
    graphics->enableMode(Graphics::ALPHA_TRANSPARENCY);
    // TODO: Implement the following line for both OpenGL and DirectX, probably with shaders
    //graphics->setColor(color);
    font->display(fontGeometry);
    graphics->disableMode(Graphics::ALPHA_TRANSPARENCY);
}
