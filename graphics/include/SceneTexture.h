#ifndef GENG_SCENE_TEXTURE_H
#define GENG_SCENE_TEXTURE_H

#include <map>
#include <string>

// TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"
#include "SelfDeletingResource.h"
#include "TextureReference.h"

/**
\brief Represents %scene images loaded in video memory.

Instances of this class represent image data that has been loaded into video memory
for the purpose of rendering the Gui.
The main difference between this class and the GuiTexture class is that textures
created in this class may have mipmaps.

This is a self-deleting resource, so never call delete on a pointer to this class.
*/
class SceneTexture : public SelfDeletingResource
{
public:
    /**
    \brief Load an image into video memory for use with the %gui system.

    It is safe to call load multiple times on the same filename.  In this case,
    load will return a pointer to the SceneTexture already created from the image.
    */
	static SceneTexture * load(const std::string & fileName);

    virtual void acquireReference();
	virtual void releaseReference();

    TextureReference *getTextureReference() { return textureRef; }

    /// Width of the image in pixels.
	int getWidth() { return width; }

    /// Height of the image in pixels.
    int getHeight() { return height; }

private:
	SceneTexture() : referenceCount(0), width(0), height(0) { }
    ~SceneTexture() { }

	static std::map<std::string, SceneTexture *> textureMap;
	std::string fileName;
	int referenceCount;
    TextureReference *textureRef;
	int width;
	int height;
};

#endif
