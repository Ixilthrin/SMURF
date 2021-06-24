#ifndef GENG_GENG_H
#define GENG_GENG_H

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN 
#endif

#ifdef WIN32
    #ifdef GENG_EXPORTS
        #include <windows.h>
        #define GENG_API __declspec(dllexport)
    #else
        #define GENG_API
    #endif
#else
    #define GENG_API
#endif

#include <string>

#ifdef __cplusplus
    extern "C" {
#endif

        // event callbacks
        typedef void (* GengWindowClosingHandler)();
        typedef void (* GengMouseMotionHandler)(int dx, int dy);
        typedef void (* GengLeftMouseButtonDownHandler)(int dx, int dy);
        typedef void (* GengRightMouseButtonDownHandler)(int dx, int dy);
        typedef void (* GengMiddleMouseButtonDownHandler)(int dx, int dy);
        typedef void (* GengLeftMouseButtonUpHandler)(int dx, int dy);
        typedef void (* GengRightMouseButtonUpHandler)(int dx, int dy);
        typedef void (* GengMiddleMouseButtonUpHandler)(int dx, int dy);
        typedef void (* GengMouseWheelRollDownHandler)();
        typedef void (* GengMouseWheelRollUpHandler)();
        typedef void (* GengKeyDownHandler)(int key);
        typedef void (* GengKeyUpHandler)(int key);
        typedef void (* GengUpdateHandler)(float deltaSeconds);
        GENG_API void gengSetWindowClosingHandler(GengWindowClosingHandler windowClosingHandler);
        GENG_API void gengSetMouseMotionHandler(GengMouseMotionHandler mouseMotionHandler);
        GENG_API void gengSetLeftMouseButtonDownHandler(GengLeftMouseButtonDownHandler leftMouseButtonDownHandler);
        GENG_API void gengSetRightMouseButtonDownHandler(GengRightMouseButtonDownHandler rightMouseButtonDownHandler);
        GENG_API void gengSetMiddleMouseButtonDownHandler(GengMiddleMouseButtonDownHandler middleMouseButtonDownHandler);
        GENG_API void gengSetLeftMouseButtonUpHandler(GengLeftMouseButtonUpHandler leftMouseButtonUpHandler);
        GENG_API void gengSetRightMouseButtonUpHandler(GengRightMouseButtonUpHandler rightMouseButtonUpHandler);
        GENG_API void gengSetMiddleMouseButtonUpHandler(GengMiddleMouseButtonUpHandler middleMouseButtonUpHandler);
        GENG_API void gengSetMouseWheelRollDownHandler(GengMouseWheelRollDownHandler mouseWheelRollDownHandler);
        GENG_API void gengSetMouseWheelRollUpHandler(GengMouseWheelRollUpHandler mouseWheelRollUpHandler);
        GENG_API void gengSetKeyDownHandler(GengKeyDownHandler keyDownHandler);
        GENG_API void gengSetKeyUpHandler(GengKeyUpHandler keyUpHandler);
        GENG_API void gengSetUpdateHandler(GengUpdateHandler updateHandler); // called once per frame

        // Event dispatcher - Required for Geng to process events and use event callbacks.
        // For SDL, pass "SDL_EVENT_PROCESSOR"
        GENG_API void gengSetEventDispatcher(std::string name);

        // game engine state
        GENG_API void gengInit(std::string windowCaption, int screenWidth, int screenHeight, 
                               std::string graphicsAPI, std::string physicsName, bool createWindow, 
                               bool useFullScreen);
        GENG_API void gengRun();
        GENG_API void gengShutdown();
        GENG_API void gengTogglePauseGameLoop();
        GENG_API void gengToggleSlowMotion();
        GENG_API void gengToggleMouseVisible();
        GENG_API void gengSetBackgroundColor(float red, float green, float blue, float alpha);
 
        // camera
        GENG_API void gengSetCameraPosition(float x, float y, float z);
        GENG_API void gengSetCameraOrientation(float yaw, float pitch);

        GENG_API void gengAvatarMoveForward();
        GENG_API void gengAvatarMoveBackward();
        GENG_API void gengAvatarStrafeLeft();
        GENG_API void gengAvatarStrafeRight();

        // entity
        GENG_API int gengEntityCreate(float x, float y, float z, float yaw);
        GENG_API void gengEntityAddModel(int entityId, const std::string & colladaFile, float scale, float xOffset, float yOffset, float zOffset, float yaw);
        //GENG_API void gengEntityAddCollisionShape(int entity, int collisionShape, float xOffset, float yOffset, float zOffset);
        GENG_API void gengEntityDestroy(int entityId);
        GENG_API void gengEntitySetPickable(int entityId, bool pickable);
        GENG_API int gengEntityPick(int screenX, int screenY);
        GENG_API void gengEntityDestroyAll();
        GENG_API void gengEntitySetPosition(int entityId, float x, float y, float z);
        GENG_API void gengEntitySetPitch(int entityId, float pitch);
        GENG_API void gengEntitySetYaw(int entityId, float yaw);
        GENG_API void gengEntitySetTexture(int entityId, std::string textureFile);
        GENG_API void gengEntitySetTotalAnimationTime(int entityId, float time);
        GENG_API int gengEntityAddController(int entityId, std::string controllerName);
        GENG_API void gengEntityRemoveController(int entityId, int controllerId);
        GENG_API void gengEntityControllerSetProperty(int entityId, int controllerId, std::string propertyName, float value);

        // actor
        GENG_API void gengEntityAddActor(int entityId);
        GENG_API void gengEntityActorAddBehavior(int entityId, std::string behaviorName);
        GENG_API void gengEntityActorSetCurrentBehavior(int entityId, std::string behaviorName);
        GENG_API void gengEntityActorSetTarget(int entityId, int targetEntityId);

		// terrain
        GENG_API void gengTerrainLoad(const char * terrainXml, int maxRigidBodies, std::string physicsName);
        GENG_API int gengTerrainCreateCollisionBox(float xHalfExtent, float yHalfExtent, float zHalfExtent);
        GENG_API void gengCreateTerrainObject(
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
            float collisionShapeYawOffset);

        // gui
        // gui screen orientations
        // If you change these, also change the corresponding values
        // in ScreenCoords.h
        #define GENG_GUI_LOWER_LEFT 1
        #define GENG_GUI_UPPER_LEFT 2 
        #define GENG_GUI_UPPER_RIGHT 3 
        #define GENG_GUI_LOWER_RIGHT 4 
        #define GENG_GUI_LOWER_CENTER 5 
        #define GENG_GUI_UPPER_CENTER 6
        #define GENG_GUI_DEAD_CENTER 7
        // gui font
        GENG_API int  gengGuiCreateFont(const char * guiFontName);
        GENG_API void gengGuiDestroyFont(int guiFontId);
        // gui text
        GENG_API int  gengGuiCreateText(int guiOrientation, int offsetX, int offsetY, int guiFontId, const char * textString);
        GENG_API void gengGuiDestroyText(int guiImageId);
        GENG_API void gengGuiSetTextVisible(int guiTextId, bool visible);
        GENG_API void gengGuiSetTextColor(int guiTextId, int red, int green, int blue, int alpha);
        GENG_API void gengGuiBringTextToFront(int guiTextId);
        GENG_API void gengGuiSetTextString(int guiTextId, const char * textString);
        // gui image
        GENG_API int  gengGuiCreateImage(int guiOrientation, int offsetX, int offsetY, const char * guiImageFileName);
        GENG_API void gengGuiDestroyImage(int guiImageId);
        GENG_API void gengGuiSetImageVisible(int guiImageId, bool visible);
        GENG_API void gengGuiSetImageSemiTransparent(int guiImageId, bool transparent);
        GENG_API void gengGuiSetImageColor(int guiImageId, int red, int green, int blue, int alpha);
        GENG_API void gengGuiBringImageToFront(int guiImageId);
        GENG_API int  gengGuiPickImage(int screenX, int screenY);
		// gui video
		GENG_API int  gengGuiCreateVideo(int guiOrientation, int offsetX, int offsetY);
		GENG_API void gengGuiDestroyVideo(int guiVideoId);
		GENG_API void gengGuiLoadVideo(int guiVideoId, const char * videoFileName);
		GENG_API void gengGuiStartVideo(int guiVideoId, bool isLooping);
		GENG_API void gengGuiPauseVideo(int guiVideoId);
		GENG_API void gengGuiResumeVideo(int guiVideoId);
		GENG_API void gengGuiUnloadVideo(int guiVideoId);


#ifdef __cplusplus
    }
#endif

#endif
