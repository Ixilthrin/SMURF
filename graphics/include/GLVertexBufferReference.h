#ifndef GENG_GLVERTEXBUFFER_REFERENCE_H
#define GENG_GLVERTEXBUFFER_REFERENCE_H

#include "opengl_includes.h"
#include "VertexBufferReference.h"
#include "Vertex1.h"
#include "Vertex2.h"
#include "Vertex5.h"

class GLVertexBufferReference : public VertexBufferReference
{
public:
	GLVertexBufferReference(int theVertexType);
	virtual ~GLVertexBufferReference();
	virtual void writeData(void *data, int dataSize);
    virtual void activate(bool picking);
	virtual void deactivate();
    virtual void destroy();
    void setId(unsigned id) { bufferId = id; }
    void setSizeOfVertex(int size) { vertexSize = size; }
private:
    int vertexCount;
    int vertexType;  // Vertex1, Vertex2, etc
    int vertexSize;
    unsigned int bufferId;
};

#endif