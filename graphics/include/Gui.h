#ifndef GENG_GUI_H
#define GENG_GUI_H

#include <list>
#include "GuiFont.h" 
#include "GuiText.h" 

class GuiOverlay;
class Camera;
class GuiImage;
//class GuiText;

class Gui
{
public:
	static Gui * getInstance();
    void init();
    void addOverlay(GuiOverlay * overlay);
    void removeOverlay(GuiOverlay * overlay);
    void update(); // Needed for text
	void draw();
    GuiImage * pick(int x, int y);

private:
	Gui();
	~Gui();

    Camera * camera;
	std::list<GuiOverlay *> overlayList;

    friend class GuiText;
    friend class GuiImage;
    friend class GuiOverlay;
};

#endif
