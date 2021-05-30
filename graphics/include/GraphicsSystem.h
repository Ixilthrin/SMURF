#ifndef GENG_GRAPHICS_SYSTEM_H
#define GENG_GRAPHICS_SYSTEM_H

#include "Color.h"
#include "ImageFileLoaderFactory.h"
#include <string>

#include "SoundExperiment1.h"

class Scene;
class SceneNode;
class btTransform;
class Graphics;
class Gui;
class WindowManager;

class GraphicsSystem
{
public:
	static GraphicsSystem * getInstance();
	~GraphicsSystem();
    void createWindow(std::string windowCaption, int desiredScreenWidth, int desiredScreenHeight, bool useFullScreen);
	Graphics * getGraphics() { return graphics; }
    Scene * getCurrentScene() { return currentScene; }
    void init(std::string graphicsAPI);
	void update(float deltaSeconds);
	void renderNextFrame();
    void setBackgroundColor(float red, float blue, float green, float alpha);
    void readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels);
//    SceneNode * createSceneNode(const std::string & entityType, float worldMatrix[], const std::string & animationName);
    SceneNode * createSceneNode(const std::string & entityType, const std::string & animationName);
    void setWindowManager(WindowManager *manager) { windowManager = manager; }
    void setImageFileLoaderFactory(ImageFileLoaderFactory *fileLoaderFactory) { imageFileLoaderFactory = fileLoaderFactory; }
    ImageFileLoaderFactory *getImageFileLoaderFactory() { return imageFileLoaderFactory; }
    
    void destroySceneNode(SceneNode * sceneNode);
    unsigned int pick(int screenX, int screenY);

private:
    GraphicsSystem() : currentScene(0), gui(0), graphics(0), 
        readingBackbuffer(0), windowManager(0), imageFileLoaderFactory(0) { }
    WindowManager *windowManager;
    ImageFileLoaderFactory *imageFileLoaderFactory;
	Graphics * graphics;
    Gui * gui;
	Scene * currentScene;
    Color backgroundColor;
    bool readingBackbuffer; // Performing an operation such as picking
    std::string graphicsAPI;

	SoundExperiment1 *sound;

    friend class SceneNode;
};

#endif
