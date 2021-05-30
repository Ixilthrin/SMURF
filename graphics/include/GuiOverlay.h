#ifndef GENG_GUI_OVERLAY_H
#define GENG_GUI_OVERLAY_H

#include "Color.h"
#include "ScreenCoords.h"

class Camera;

class GuiOverlay
{
public:
	virtual ~GuiOverlay();
    virtual void update() = 0;
	virtual void draw() =0;
    virtual bool pick(int x, int y);
//    virtual void setColor(const Color & color) { this->color = color; }
    virtual void setColor(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha) { color = (red << 24) | (green << 16) | (blue << 8) | (alpha << 0); }
    bool getVisible() { return visible; }
    void setVisible(bool visible);
    void bringToFront();
    ScreenCoords getScreenCoords() { return screenCoords; }
    void setSemiTransparent(bool transparent) { isSemiTransparent = true; }

protected:
    GuiOverlay() : visible(true), color(0xFFFFFFFF), isSemiTransparent(0) { }

    bool visible;
//    Color color;
    unsigned int color;
    // absolute screen coordinates
    ScreenCoords screenCoords;
    bool isSemiTransparent;
};

#endif
