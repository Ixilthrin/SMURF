#include "LinearMath/btTransform.h"
#ifdef WIN32
#include "D3DGraphics.h"
#include "D3D_SDL_WindowManager.h"
#include "GL_SDL_WindowManager.h"
#endif

#include "GLGraphics.h"


#include "GraphicsSystem.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Model.h"
#include "Gui.h"
#include "util.h"
#include "WindowManager.h"

GraphicsSystem * GraphicsSystem::getInstance()
{
    static GraphicsSystem *instance = new GraphicsSystem();
    return instance;
}

GraphicsSystem::~GraphicsSystem()
{
}

void GraphicsSystem::init(std::string theGraphicsAPI)
{
	sound = new SoundExperiment1();
	sound->setup();

    graphicsAPI = theGraphicsAPI;
    if (graphicsAPI == std::string("Direct3D9"))
    {
#ifdef WIN32
        graphics = D3DGraphics::getInstance();
#else
        fatalError("Direct3D not supported on this system.");
#endif
    }
    else if (graphicsAPI == std::string("OpenGL"))
    {
        graphics = GLGraphics::getInstance();
    }
    else
    {
        fatalError(graphicsAPI + " is not a supported graphics API");
    }
    currentScene = new Scene(graphics);
    gui = Gui::getInstance();
}

void GraphicsSystem::createWindow(std::string windowCaption, int desiredScreenWidth, int desiredScreenHeight, bool useFullScreen)
{
    if (desiredScreenWidth == 0 && desiredScreenHeight == 0)
        useFullScreen = true;

    if (!windowManager)
    {
        fatalError("A WindowManager object is not set for GraphicsSystem.");
    }
    windowManager->createWindow();
    graphics->setScreenWidth(windowManager->getScreenWidth());
    graphics->setScreenHeight(windowManager->getScreenHeight());
}

void GraphicsSystem::update(float deltaSeconds)
{
    currentScene->update(deltaSeconds);
}

void GraphicsSystem::renderNextFrame()
{
    graphics->clearDepthAndScreenBuffers();
    
	gui->update();
    graphics->startRenderingPhase();

    graphics->setBackgroundColor(backgroundColor.red, backgroundColor.blue, backgroundColor.green, backgroundColor.alpha);

    currentScene->draw();
    //// draw terrain
    //gui->draw();

    graphics->stopRenderingPhase();

    // Do not swap if picking (picking reads the backbuffer)
    if (!readingBackbuffer)
    {
        windowManager->swapBuffers();
    }
    readingBackbuffer = false;
}

void GraphicsSystem::setBackgroundColor(float red, float blue, float green, float alpha)
{
    backgroundColor.red = red;
    backgroundColor.blue = blue;
    backgroundColor.green = green;
    backgroundColor.alpha = alpha;
}

void GraphicsSystem::readFromBackBuffer(int x, int y, int width, int height, unsigned char *pixels)
{
    readingBackbuffer = true;
    graphics->readFromBackBuffer(x, y, width, height, pixels);
}

SceneNode * GraphicsSystem::createSceneNode(const std::string & colladaFile, const std::string & animationName)
{
    SceneNode * sceneNode = new SceneNode();
    Model * model = Model::create(colladaFile);
    sceneNode->drawable = model;
    currentScene->addSceneNode(sceneNode);
    return sceneNode;
}

void GraphicsSystem::destroySceneNode(SceneNode * sceneNode)
{
    std::vector<SceneNode *> sceneNodes = currentScene->sceneNodes;
    std::vector<SceneNode *>::iterator iter = sceneNodes.begin();
    std::vector<SceneNode *>::iterator end = sceneNodes.end();
    for (; iter != end; ++iter)
    {
        if (*iter == sceneNode)
        {
            sceneNodes.erase(iter);
            break;
        }
    }
    delete sceneNode;
}

unsigned int GraphicsSystem::pick(int screenX, int screenY)
{
    static int currentSound = 0;
	// commented this to try to avoid an exception, need to fix
	//if (sound) {
	//    sound->play(currentSound);
	//	currentSound++;
	//	if (currentSound > 4) 
	//		currentSound = 0;
	//}
    return currentScene->pick(screenX, screenY);
}
