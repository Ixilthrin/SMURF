#include "GLIndexBufferReference.h"
#include "IndexBufferInfo.h"
#include "GLGraphics.h"

GLIndexBufferReference::GLIndexBufferReference(GLGraphics *graphics, int numIndices)
: graphicsObject(graphics), bufferId(0), indexCount(numIndices)
{
}

GLIndexBufferReference::~GLIndexBufferReference()
{
    destroy();
}

void GLIndexBufferReference::activate()
{
#ifndef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
    glEnableClientState(GL_INDEX_ARRAY);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
    if (indexType == IndexBufferInfo::INT_16_BIT_INDICES)
    {
        graphicsObject->setIndexBufferType(GL_UNSIGNED_SHORT);
    }
    else
    {
        graphicsObject->setIndexBufferType(GL_UNSIGNED_INT);
    }
}

void GLIndexBufferReference::deactivate()
{
#ifndef USING_OPENGL_WITH_IPHONE  // Defined in opengl_includes.h
    glDisableClientState(GL_INDEX_ARRAY);
#endif
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GLIndexBufferReference::destroy()
{
    glDeleteBuffers(1, &bufferId);
}
