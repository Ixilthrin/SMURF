#include "Scene.h"

#include "Camera.h"
#include "Drawable.h"
#include "Graphics.h"
#include "GraphicsSystem.h"
#include "SceneNode.h"

Scene::Scene(Graphics *g)
: graphics(g), sceneNodes()
{
}

void Scene::update(float deltaSeconds)
{
    // Update all of the scene nodes.
    std::vector<SceneNode *>::iterator it = sceneNodes.begin();
	std::vector<SceneNode *>::iterator end = sceneNodes.end();
	for (; it != end; it++)
	{
        (*it)->update(deltaSeconds);
	}
}

void Scene::draw()
{
    graphics->enableMode(Graphics::CULLING);
    
    Camera::getInstance()->apply();
    
    // Draw all of the scene nodes
    std::vector<SceneNode *>::iterator it = sceneNodes.begin();
	std::vector<SceneNode *>::iterator end = sceneNodes.end();
	for (; it != end; it++)
	{
		(*it)->draw(graphics, false);
	}
	//std::string errorMessage;
	//graphics->checkError(errorMessage);
}

unsigned int Scene::pick(int screenX, int screenY)
{
    Camera * camera = Camera::getInstance();
    graphics->setAttributesFor3DPicking();
    camera->apply();
    std::vector<SceneNode *>::iterator it = sceneNodes.begin();
	std::vector<SceneNode *>::iterator end = sceneNodes.end();
	for (; it != end; it++)
	{
		(*it)->draw(graphics, true);
	}

	unsigned char pixel[3];
    ScreenPosition pos = graphics->getAbsoluteScreenPosition(screenX, screenY);
    GraphicsSystem *graphicsSystem = GraphicsSystem::getInstance();
    graphicsSystem->readFromBackBuffer(pos.x, pos.y, 1, 1, pixel);
	unsigned int pickedColor = pixel[0] | (pixel[1] << 8) | (pixel[2] << 16);

    return pickedColor;
}
