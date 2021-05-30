#include "GLTextureReference.h"

#include "opengl_includes.h"

GLTextureReference::GLTextureReference(int theWidth, int theHeight, int thePixelFormat, bool dynamic)
: width(theWidth), height(theHeight), pixelFormat(thePixelFormat), isDynamic(dynamic)
{
}

GLTextureReference::~GLTextureReference()
{
}

void GLTextureReference::activate()
{
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void GLTextureReference::deactivate()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTextureReference::destroy()
{
    glDeleteTextures(1, &textureId);
}

/**
 *   This will only be used when using dynamic textures.
 *   @data The raw RGB or RGBA data
 *   @dataSize The size of the data in bytes
 */
void GLTextureReference::writeData(void *data, int dataSize)
{
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, pixelFormat, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}