#ifndef GENG_GUI_TEXT_H
#define GENG_GUI_TEXT_H

#include <string>
#include "GuiOverlay.h"

class GuiText : public GuiOverlay
{
public:
    GuiText(int guiOrientation, int offsetX, int offsetY, 
            GuiFont * font, const std::string & textString);
    virtual ~GuiText();
    virtual void update();
    virtual void draw();
    virtual bool pick(int x, int y) { return false; }
    void setTextString(const std::string & textString);
    std::string & getTextString() { return textString; }

private:
    GuiFont * font;
    FontGeometry *fontGeometry;
    std::string textString;
    bool textHasChanged;
    bool positionHasChanged;
};

#endif
