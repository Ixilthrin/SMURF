#include "GLVertexBufferReference.h"
#include "GLGraphics.h"
#include "VertexBufferInfo.h"

GLVertexBufferReference::GLVertexBufferReference(int theVertexType)
: vertexType(theVertexType), bufferId(0), vertexSize(0)
{
}

GLVertexBufferReference::~GLVertexBufferReference()
{
    destroy();
}

void GLVertexBufferReference::writeData(void *data, int dataSize)
{    
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, dataSize, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, dataSize, data);
}

void GLVertexBufferReference::activate(bool picking)
{
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    
    if (vertexType == VertexBufferInfo::VERTEX2)
    {
        if (!picking)
        {
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));
    } 
    else if (vertexType == VertexBufferInfo::VERTEX1)
    {
        if (!picking)
        {
            glClientActiveTexture(GL_TEXTURE0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));
    }
    else if (vertexType == VertexBufferInfo::VERTEX5)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        if (!picking)
        {
            glClientActiveTexture(GL_TEXTURE0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(24));
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));
    }
    else if (vertexType == VertexBufferInfo::VERTEX8)
    {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, vertexSize, BUFFER_OFFSET(12));
        if (!picking)
        {
            glClientActiveTexture(GL_TEXTURE0);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(24));
            glClientActiveTexture(GL_TEXTURE1);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(32));
            glClientActiveTexture(GL_TEXTURE2);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, vertexSize, BUFFER_OFFSET(40));
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, vertexSize, BUFFER_OFFSET(0));
    }
}

void GLVertexBufferReference::deactivate()
{
    glDisableClientState(GL_NORMAL_ARRAY);
    //glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    if (vertexType == VertexBufferInfo::VERTEX8)
    {
        //glClientActiveTexture(GL_TEXTURE1);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        //glClientActiveTexture(GL_TEXTURE2);
        //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GLVertexBufferReference::destroy()
{
    glDeleteBuffers(1, &bufferId);
}