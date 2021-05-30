#ifndef GENG_GUI_TEXTURE_H
#define GENG_GUI_TEXTURE_H

#include <map>
#include <string>

// TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"
#include "SelfDeletingResource.h"
#include "TextureReference.h"

/**
\brief Represents %gui images loaded in video memory.

Instances of this class represent image data that has been loaded into video memory
for the purpose of rendering the Gui.
The main difference between this class and the SceneTexture class is that textures
created in this class do not have mipmaps.

This is a self-deleting resource, so never call delete on a pointer to this class.
*/
class GuiTexture : public SelfDeletingResource
{
public:
    /**
    \brief Load an image into video memory for use with the %gui system.

    It is safe to call load multiple times on the same filename.  In this case,
    load will return a pointer to the GuiTexture already created from the image.
    */
	static GuiTexture * load(const std::string & fileName);

    virtual void acquireReference();
	virtual void releaseReference();

    TextureReference *getTextureReference() { return textureRef; }

    /// Width of the image in pixels.
	int getWidth() { return width; }

    /// Height of the image in pixels.
    int getHeight() { return height; }
    
    float getTextureCoordUMax() { return textureCoordUMax; }
    float getTextureCoordVMax() { return textureCoordVMax; }

private:
	GuiTexture() : referenceCount(0), width(0), height(0),
                   textureCoordUMax(1.0), textureCoordVMax(1.0) { }
    ~GuiTexture() { }

	static std::map<std::string, GuiTexture *> textureMap;
	std::string fileName;
	int referenceCount;
    TextureReference *textureRef;
	int width;
	int height;
    float textureCoordUMax;  // Used when adding padding to image. ( = 1.0 if no padding)
    float textureCoordVMax;  // Used when adding padding to image. ( = 1.0 if no padding)
};

#endif
