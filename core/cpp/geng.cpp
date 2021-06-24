#include <iostream>
#include "geng.h"
#include "SDLEventDispatcher.h"
#include "Color.h"
#include "Camera.h"
#include "LinearMath/btVector3.h"
#include "util.h"
#include "Gui.h"
#include "GuiFont.h"
#include "GuiImage.h"
#include "GuiImageCreator.h"
#include "GuiImageInfo.h"
#include "GuiOverlay.h"

// TODO: Enable video for Mac
#ifndef __APPLE__
#include "GuiVideo.h"
#endif

#include "Actor.h"
#include "Entity.h"
#include "SceneNode.h"
#include "GraphicsSystem.h"
#include "Graphics.h"
#include "Area.h"
#include "Avatar.h"
#include "SDL.h"

#include "D3D_SDL_WindowManager.h"
#include "GL_SDL_WindowManager.h"
#include "SDLImageFileLoaderFactory.h"

static EventDispatcher * eventDispatcher = 0;
static Camera * camera = Camera::getInstance();
static Gui * gui = Gui::getInstance();
static GraphicsSystem * graphicsSystem = GraphicsSystem::getInstance();
static Graphics * graphics = 0;
static bool running = true;
static GengUpdateHandler updateHandler = 0;
static unsigned long previousMilliseconds = 0;
static bool gameLoopIsPaused = false;
static bool slowMotion = false;

std::map<int, Entity *> entityIdMap;
std::map<Entity *, int> entityMap;

std::map<int, GuiFont *> guiFontIdMap;
std::map<int, GuiText *> guiTextIdMap;
std::map<int, GuiImage *> guiImageIdMap;
std::map<GuiImage *, int> guiImageMap;

#ifndef __APPLE__
std::map<int, GuiVideo *> guiVideoIdMap;
#endif

Area * currentArea = 0;
Avatar * avatar = 0;

// event dispatcher
void gengSetEventDispatcher(std::string name)
{
    if (name == std::string("SDL_EVENT_DISPATCHER"))
    {
        eventDispatcher = new SDLEventDispatcher();
        eventDispatcher->init();
    }
    else
    {
        fatalError("Event dispatcher not recognized");
    }
}

static void checkEventDispatcher()
{
    if (!eventDispatcher)
    {
        fatalError("No event dispatcher set");
    }
}

// event callbacks

void gengSetWindowClosingHandler(GengWindowClosingHandler windowClosingHandler) 
{
    checkEventDispatcher();
    eventDispatcher->setWindowClosingHandler(windowClosingHandler); 
}

void gengSetMouseMotionHandler(GengMouseMotionHandler mouseMotionHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setMouseMotionHandler(mouseMotionHandler); 
}

void gengSetLeftMouseButtonDownHandler(GengLeftMouseButtonDownHandler leftMouseButtonDownHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setLeftMouseButtonDownHandler(leftMouseButtonDownHandler); 
}

void gengSetRightMouseButtonDownHandler(GengRightMouseButtonDownHandler rightMouseButtonDownHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setRightMouseButtonDownHandler(rightMouseButtonDownHandler); 
}

void gengSetMiddleMouseButtonDownHandler(GengMiddleMouseButtonDownHandler middleMouseButtonDownHandler) 
{ 
    eventDispatcher->setMiddleMouseButtonDownHandler(middleMouseButtonDownHandler); 
}

void gengSetLeftMouseButtonUpHandler(GengLeftMouseButtonUpHandler leftMouseButtonUpHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setLeftMouseButtonUpHandler(leftMouseButtonUpHandler); 
}

void gengSetRightMouseButtonUpHandler(GengRightMouseButtonUpHandler rightMouseButtonUpHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setRightMouseButtonUpHandler(rightMouseButtonUpHandler); 
}

void gengSetMiddleMouseButtonUpHandler(GengMiddleMouseButtonUpHandler middleMouseButtonUpHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setMiddleMouseButtonUpHandler(middleMouseButtonUpHandler); 
}

void gengSetMouseWheelRollDownHandler(GengMouseWheelRollDownHandler mouseWheelRollDownHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setMouseWheelRollDownHandler(mouseWheelRollDownHandler); 
}

void gengSetMouseWheelRollUpHandler(GengMouseWheelRollUpHandler mouseWheelRollUpHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setMouseWheelRollUpHandler(mouseWheelRollUpHandler); 
}

void gengSetKeyDownHandler(GengKeyDownHandler keyDownHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setKeyDownHandler(keyDownHandler); 
}

void gengSetKeyUpHandler(GengKeyUpHandler keyUpHandler) 
{ 
    checkEventDispatcher();
    eventDispatcher->setKeyUpHandler(keyUpHandler); 
}

void gengSetUpdateHandler(GengUpdateHandler gengUpdateHandler)
{
    updateHandler = gengUpdateHandler;
}

// game engine state

void gengInit(std::string windowCaption, int desiredScreenWidth, int desiredScreenHeight, 
              std::string graphicsAPI, std::string physicsName, 
              bool createWindow, bool useFullScreen)
{
	currentArea = new Area(physicsName);
    graphicsSystem->init(graphicsAPI);
    graphicsSystem->setImageFileLoaderFactory(new SDLImageFileLoaderFactory());
    if (graphicsAPI == std::string("OpenGL"))
    {
        graphicsSystem->setWindowManager(
            new GL_SDL_WindowManager(windowCaption, desiredScreenWidth, desiredScreenHeight, useFullScreen));
    }
    else
    {
        graphicsSystem->setWindowManager(
            new D3D_SDL_WindowManager(windowCaption, desiredScreenWidth, desiredScreenHeight, useFullScreen));
    }
    graphics = graphicsSystem->getGraphics();
    if (useFullScreen)
    {
        graphics->setFullScreen(true);
    }
    if (createWindow)
    {
        graphicsSystem->createWindow(windowCaption, desiredScreenWidth, desiredScreenHeight, useFullScreen);
    }
    graphics->initialize();
    gui->init();
    if (eventDispatcher)
        eventDispatcher->init();
}

void gengRun()
{
	graphicsSystem->setBackgroundColor(1.0f, 0.0f, 0.0f, 0.0f);
    if (currentArea == 0)
    {
        fatalError("Must load area before calling gengRun()");
    }
    unsigned long previousMilliseconds = SDL_GetTicks();
    while (running)
    {
        unsigned long currentMilliseconds = SDL_GetTicks();
        unsigned long deltaMilliseconds = currentMilliseconds - previousMilliseconds;
        if (deltaMilliseconds == 0)
        {
            continue;
        }
        previousMilliseconds = currentMilliseconds;
        float deltaSeconds = static_cast<float>(deltaMilliseconds / 1000.0f);
        if (!gameLoopIsPaused)
        {
            if (slowMotion)
            {
                currentArea->update(deltaSeconds/5.0f);
                graphicsSystem->update(deltaSeconds/5.0f);
            }
            else
            {
                currentArea->update(deltaSeconds);
                graphicsSystem->update(deltaSeconds);
            }
        }
        if (updateHandler != 0)
        {
            updateHandler(deltaSeconds);
        }
        if (eventDispatcher)
        {
            eventDispatcher->processEvents();
        }
	    graphicsSystem->renderNextFrame();
    }
}

void gengToggleMouseVisible()
{
    eventDispatcher->toggleMouseVisible();
}

void gengShutdown()
{
    running = false;
}

void gengTogglePauseGameLoop()
{
    gameLoopIsPaused = !gameLoopIsPaused;
}

void gengToggleSlowMotion()
{
    slowMotion = !slowMotion;
}

void gengSetBackgroundColor(float red, float green, float blue, float alpha)
{
	graphicsSystem->setBackgroundColor(red, green, blue, alpha);
}

// camera

void gengSetCameraPosition(float x, float y, float z)
{
    camera->setPosition(x, y, z);
}

void gengSetCameraOrientation(float yaw, float pitch)
{
    camera->setOrientation(yaw, pitch);
}

void gengAvatarMoveForward()
{
    avatar->moveForward();
}

void gengAvatarMoveBackward()
{
    avatar->moveBackward();
}

void gengAvatarStrafeLeft()
{
    avatar->strafeLeft();
}

void gengAvatarStrafeRight()
{
    avatar->strafeRight();
}

// entity

int grabNextUnallocatedId() 
{ 
    static int nextUnallocatedGengId = 0;
    return ++nextUnallocatedGengId; 
}

int gengEntityCreate(float x, float y, float z, float yaw)
{
    int entityId = grabNextUnallocatedId();
    Entity * entity = currentArea->createEntity(btVector3(x, y, z), yaw);
    entityIdMap[entityId] = entity;
    entityMap[entity] = entityId;
	return entityId;
}

void gengEntityAddModel(int entityId, const std::string & colladaFile, float scale, float xOffset, float yOffset, float zOffset, float yaw)
{
    //assertFileExists(colladaFile);
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityAddModel called with invalid id");
    }
    Entity * entity = entityIdMap[entityId];
    entity->addModel(colladaFile, scale, xOffset, yOffset, zOffset, yaw);
}

void gengEntityDestroy(int entityId)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityDestroy called with invalid id");
    }
    Entity * entity = entityIdMap[entityId];
    entityMap.erase(entity);
    entityIdMap.erase(entityId);
    currentArea->destroyEntity(entity);
}

void gengEntitySetPickable(int entityId, bool pickable)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntitySetPickable called with invalid id");
    }
    entityIdMap[entityId]->setPickable(pickable);
}

int gengEntityPick(int screenX, int screenY)
{
    Entity * entity = currentArea->pick(screenX, screenY);
    if (entity == 0)
    {
        return 0;
    }
    return entityMap[entity];
}

void gengEntityDestroyAll()
{
    fatalError("gengEntityDestroyAll not yet implemented");
}

void gengEntitySetPosition(int entityId, float x, float y, float z)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntitySetPosition called with invalid id");
    }
    entityIdMap[entityId]->setPosition(btVector3(x, y, z));
}

void gengEntitySetPitch(int entityId, float pitch)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntitySetPitch called with invalid id");
    }
    entityIdMap[entityId]->setPitch((float) (pitch * (float) M_PI / 180.0f));
}

void gengEntitySetYaw(int entityId, float yaw)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntitySetYaw called with invalid id");
    }
    entityIdMap[entityId]->setYaw((float) (yaw * (float) M_PI / 180.0f));
}

void gengEntitySetTexture(int entityId, std::string textureFile)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntitySetTexture called with invalid id");
    }
    entityIdMap[entityId]->setTexture(textureFile);
}

void gengEntitySetTotalAnimationTime(int entityId, float time)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntitySetTotalAnimationTime called with invalid id");
    }
    entityIdMap[entityId]->setTotalAnimationTime(time);
}

int gengEntityAddController(int entityId, std::string controllerName)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityAddController called with invalid id");
    }
    int controllerId = grabNextUnallocatedId();
    entityIdMap[entityId]->addControllerByName(controllerName, controllerId);
    return controllerId;
}

void gengEntityRemoveController(int entityId, int controllerId)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityRemoveController called with invalid id");
    }
    entityIdMap[entityId]->removeController(controllerId);
}

void gengEntityControllerSetProperty(int entityId, int controllerId, std::string propertyName, float value)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityControllerSetProperty called with invalid id");
    }
    entityIdMap[entityId]->setControllerProperty(controllerId, propertyName, value);
}
        
// actor
void gengEntityAddActor(int entityId)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityAddActor called with invalid id");
    }
    Entity * entity = entityIdMap[entityId];
    Actor *actor = new Actor(entity);
    entity->setActor(actor);
}

void gengEntityActorAddBehavior(int entityId, std::string behaviorName)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityActorAddBehavior called with invalid id");
    }
    Entity * entity = entityIdMap[entityId];
    Actor *actor = entity->getActor();
    if (!actor)
    {
        fatalError("Adding behavior to entity that does not have an actor.");
    }
    actor->addBehavior(behaviorName);
}

void gengEntityActorSetCurrentBehavior(int entityId, std::string behaviorName)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityActorSetCurrentBehavior called with invalid id");
    }
    Entity * entity = entityIdMap[entityId];
    Actor *actor = entity->getActor();
    if (!actor)
    {
        fatalError("Setting current behavior to entity that does not have an actor.");
    }
    actor->setCurrentBehavior(behaviorName);
}

void gengEntityActorSetTarget(int entityId, int targetEntityId)
{
    if (entityIdMap.count(entityId) == 0)
    {
        fatalError("gengEntityActorSetTarget called with invalid id.");
    }
    if (entityIdMap.count(targetEntityId) == 0)
    {
        fatalError("gengEntityActorSetTarget called with invalid id for target.");
    }
    Entity *entity = entityIdMap[entityId];
    Actor *entityActor = entity->getActor();
    if (!entityActor)
    {
        fatalError("Attempting to set target of entity that does not have an actor.");
    }
    Entity *targetEntity = entityIdMap[targetEntityId];
    Actor *targetActor = targetEntity->getActor();
    if (!targetActor)
    {
        fatalError("Attempting to set entity target actor that does not exist.");
    }
    entityActor->setTarget(targetActor);
}

// terrain

void gengTerrainLoad(const char * terrainXml, int maxRigidBodies, std::string physicsName)
{
    delete currentArea;
    currentArea = new Area(physicsName, terrainXml, maxRigidBodies);
    avatar = currentArea->getAvatar();
}

int gengTerrainCreateCollisionBox(float xHalfExtent, float yHalfExtent, float zHalfExtent)
{
    int id = grabNextUnallocatedId();
    currentArea->createCollisionBox(id, xHalfExtent, yHalfExtent, zHalfExtent);
    return id;
}

void gengCreateTerrainObject(
                             float x, 
                             float y, 
                             float z, 
                             float yaw, 
                             const std::string & colladaFile, 
                             float renderScale,
                             float renderXOffset,
                             float renderYOffset,
                             float renderZOffset,
                             float renderYawOffset, 
                             int collisionShapeId, 
                             float collisionShapeXOffset,
                             float collisionShapeYOffset,
                             float collisionShapeZOffset,
                             float collisionShapeYawOffset)
{
    currentArea->createTerrainObject(x, y, z, yaw, colladaFile, renderScale,
                                     renderXOffset, renderYOffset, renderZOffset,
                                     renderYawOffset, collisionShapeId, collisionShapeXOffset,
                                     collisionShapeYOffset, collisionShapeZOffset,
                                     collisionShapeYawOffset);

}

// gui

// gui font 
int gengGuiCreateFont(const char * guiFontName)
{
    int guiFontId = grabNextUnallocatedId();
    GuiFont * guiFont = GuiFont::create(guiFontName);
    guiFontIdMap[guiFontId] = guiFont;
    return guiFontId;
}

void gengGuiDestroyFont(int guiFontId)
{
    if (guiFontIdMap.count(guiFontId) == 0)
    {
        fatalError("gengDestroyGuiFont called with invalid id");
    }
    guiFontIdMap[guiFontId]->releaseReference();
    guiFontIdMap.erase(guiFontId);
}

// gui text

int gengGuiCreateText(int guiOrientation, int offsetX, int offsetY, int guiFontId, const char * textString)
{
    if (guiFontIdMap.count(guiFontId) == 0)
    {
        fatalError("gengGuiCreateText called with invalid id");
    }
    GuiFont * guiFont = guiFontIdMap[guiFontId];
    int guiTextId = grabNextUnallocatedId();
    GuiText * guiText = new GuiText(guiOrientation, offsetX, offsetY, guiFont, textString);
    guiTextIdMap[guiTextId] = guiText;
    return guiTextId;
}

void gengGuiDestroyText(int guiTextId)
{
    if (guiTextIdMap.count(guiTextId) == 0)
    {
        fatalError("gengGuiDestroyText called with invalid id");
    }
    GuiText * guiText = guiTextIdMap[guiTextId];
    guiTextIdMap.erase(guiTextId);
    delete guiText;
}

void gengGuiSetTextVisible(int guiTextId, bool visible)
{
    if (guiTextIdMap.count(guiTextId) == 0)
    {
        fatalError("gengGuiSetTextVisible called with invalid id");
    }
    GuiText * guiText = guiTextIdMap[guiTextId];
    guiText->setVisible(visible);
}

void gengGuiSetTextColor(int guiTextId, int red, int green, int blue, int alpha)
{
    if (guiTextIdMap.count(guiTextId) == 0)
    {
        fatalError("gengGuiSetTextColor called with invalid id");
    }
    GuiText * guiText = guiTextIdMap[guiTextId];
//    guiText->setColor(Color(red, green, blue, alpha));
    guiText->setColor(red, green, blue, alpha);
}

void gengGuiBringTextToFront(int guiTextId)
{
    if (guiTextIdMap.count(guiTextId) == 0)
    {
        fatalError("gengGuiBringTextToFront called with invalid id");
    }
    GuiText * guiText = guiTextIdMap[guiTextId];
    guiText->bringToFront();
}

void gengGuiSetTextString(int guiTextId, const char * textString)
{
    if (guiTextIdMap.count(guiTextId) == 0)
    {
        fatalError("gengGuiSetTextString called with invalid id");
    }
    GuiText * guiText = guiTextIdMap[guiTextId];
    if (guiText->getTextString() != textString)
    {
        guiText->setTextString(textString);
    }
}

// gui image

int gengGuiCreateImage(int guiOrientation, int offsetX, int offsetY, const char * guiImageFileName)
{
    int guiImageId = grabNextUnallocatedId();

    GuiImageCreator *imageCreator = graphics->getGuiImageCreator();
    GuiImageInfo *info = new GuiImageInfo(guiOrientation, offsetX, offsetY, guiImageFileName);
    GuiImage *guiImage = imageCreator->createGuiImage(info);

    // Old code, before GuiImageCreator.  Remove when confident no reference to this is necessary.
    //GuiImage * guiImage = new GuiImage(guiOrientation, offsetX, offsetY, guiImageFileName);

    guiImageIdMap[guiImageId] = guiImage;
    guiImageMap[guiImage] = guiImageId;
    return guiImageId;
}

void gengGuiDestroyImage(int guiImageId)
{
    if (guiImageIdMap.count(guiImageId) == 0)
    {
        fatalError("gengGuiDestroyImage called with invalid id");
    }
    GuiImage * guiImage = guiImageIdMap[guiImageId];
    guiImageIdMap.erase(guiImageId);
    guiImageMap.erase(guiImage);
    delete guiImage;
}

void gengGuiSetImageVisible(int guiImageId, bool visible)
{
    if (guiImageIdMap.count(guiImageId) == 0)
    {
        fatalError("gengGuiSetImageVisible called with invalid id");
    }
    GuiImage * guiImage = guiImageIdMap[guiImageId];
    guiImage->setVisible(visible);
}

void gengGuiSetImageSemiTransparent(int guiImageId, bool transparent)
{
    if (guiImageIdMap.count(guiImageId) == 0)
    {
        fatalError("gengGuiSetImageSemiTransparent called with invalid id");
    }
    GuiImage * guiImage = guiImageIdMap[guiImageId];
    guiImage->setSemiTransparent(transparent);
}

void gengGuiSetImageColor(int guiImageId, int red, int green, int blue, int alpha)
{
    if (guiImageIdMap.count(guiImageId) == 0)
    {
        fatalError("gengGuiSetImageColor called with invalid id");
    }
    if (red < 0 || green < 0 || blue < 0 || alpha < 0 || red > 255 || green > 255 || blue > 255 || alpha > 255)
    {
        fatalError("gengGuiSetImageColor error: colors values should be between 0 and 255 inclusive");
    }
    GuiImage * guiImage = guiImageIdMap[guiImageId];
//    guiImage->setColor(Color(red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f));
    guiImage->setColor(red, green, blue, alpha);
}

void gengGuiBringImageToFront(int guiImageId)
{
    if (guiImageIdMap.count(guiImageId) == 0)
    {
        fatalError("gengGuiBringImageToFront called with invalid id");
    }
    GuiImage * guiImage = guiImageIdMap[guiImageId];
    guiImage->bringToFront();
}

int gengGuiPickImage(int screenX, int screenY)
{
    GuiImage * guiImage = gui->pick(screenX, screenY);
    if (guiImage != 0)
    {
        return guiImageMap[guiImage];
    }
    else
    {
        return 0;
    }
}

// gui video

int gengGuiCreateVideo(int guiOrientation, int offsetX, int offsetY)
{
#ifndef __APPLE__
    int guiVideoId = grabNextUnallocatedId();
    GuiVideo * guiVideo = new GuiVideo(guiOrientation, offsetX, offsetY);
    guiVideoIdMap[guiVideoId] = guiVideo;
    return guiVideoId;
#else
    return 0;
#endif
}

void gengGuiDestroyVideo(int guiVideoId)
{
#ifndef __APPLE__
    if (guiVideoIdMap.count(guiVideoId) == 0)
    {
        fatalError("gengGuiDestroyVideo called with invalid id");
    }
    GuiVideo * guiVideo = guiVideoIdMap[guiVideoId];
    guiVideo->unload(); // call in case video is not unloaded
    guiVideoIdMap.erase(guiVideoId);
    delete guiVideo;
#endif

}

void gengGuiLoadVideo(int guiVideoId, const char * videoFileName)
{
#ifndef __APPLE__
    if (guiVideoIdMap.count(guiVideoId) == 0)
    {
        fatalError("gengGuiLoadVideo called with invalid id");
    }
    GuiVideo * guiVideo = guiVideoIdMap[guiVideoId];
    guiVideo->load(videoFileName);
#endif

}

void gengGuiStartVideo(int guiVideoId, bool isLooping)
{
#ifndef __APPLE__
    if (guiVideoIdMap.count(guiVideoId) == 0)
    {
        fatalError("gengGuiStartVideo called with invalid id");
    }
    GuiVideo * guiVideo = guiVideoIdMap[guiVideoId];
    guiVideo->start(isLooping);
#endif

}

void gengGuiPauseVideo(int guiVideoId)
{
#ifndef __APPLE__
    if (guiVideoIdMap.count(guiVideoId) == 0)
    {
        fatalError("gengGuiPauseVideo called with invalid id");
    }
    GuiVideo * guiVideo = guiVideoIdMap[guiVideoId];
    guiVideo->pause();
#endif

}

void gengGuiResumeVideo(int guiVideoId)
{
#ifndef __APPLE__
    if (guiVideoIdMap.count(guiVideoId) == 0)
    {
        fatalError("gengGuiResumeVideo called with invalid id");
    }
    GuiVideo * guiVideo = guiVideoIdMap[guiVideoId];
    guiVideo->resume();
#endif

}

void gengGuiUnloadVideo(int guiVideoId)
{
#ifndef __APPLE__
    if (guiVideoIdMap.count(guiVideoId) == 0)
    {
        fatalError("gengGuiUnloadVideo called with invalid id");
    }
    GuiVideo * guiVideo = guiVideoIdMap[guiVideoId];
    guiVideo->unload();
#endif

}

