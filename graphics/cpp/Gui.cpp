#include <cassert>
#include <sstream>
#include "util.h"
#include "Camera.h"
#include "Gui.h"
#include "GuiTexture.h"
#include "GuiOverlay.h"
#include "Graphics.h"
#include "GraphicsSystem.h"
//#define USE_FFMPEG
#ifdef USE_FFMPEG
extern "C" {
  #include <libavformat/avformat.h>
}
#endif

Gui * Gui::getInstance() { static Gui instance; return &instance; }

Gui::Gui() : camera(0)
{
}

Gui::~Gui()
{
}

void Gui::init()
{
    camera = Camera::getInstance();
#ifdef USE_FFMPEG
    av_register_all();
#endif
}

void Gui::removeOverlay(GuiOverlay * overlay)
{
	overlayList.remove(overlay);
}

void Gui::addOverlay(GuiOverlay * overlay)
{
	overlayList.push_back(overlay);
}

GuiImage * Gui::pick(int x, int y)
{
	std::list<GuiOverlay *>::iterator it = overlayList.begin();
	std::list<GuiOverlay *>::iterator end = overlayList.end();
	while (it != end)
	{
		GuiOverlay * overlay = *it;
		if (overlay->pick(x, y))
        {
            return reinterpret_cast<GuiImage *>(overlay);
        }
		++it;
	}
    return 0;
}

void Gui::update()
{
	std::list<GuiOverlay *>::iterator it = overlayList.begin();
	std::list<GuiOverlay *>::iterator end = overlayList.end();
	while (it != end)
	{
		(*it)->update();
		++it;
	}
}

void Gui::draw()
{
    Graphics * graphics = GraphicsSystem::getInstance()->getGraphics();

	//glDepthMask(GL_FALSE);  // This breaks 3d drawing even when I set it back to true.
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    graphics->setMatrixMode(Graphics::PROJECTION);
    graphics->pushMatrix();
    graphics->setGuiProjection();
    graphics->setMatrixMode(Graphics::MODELVIEW);
    graphics->setIdentityMatrix();

	std::list<GuiOverlay *>::iterator it = overlayList.begin();
	std::list<GuiOverlay *>::iterator end = overlayList.end();
	while (it != end)
	{
		(*it)->draw();
		++it;
	}
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

