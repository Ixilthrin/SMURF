#ifndef GENG_GLINDEXBUFFER_REFERENCE_H
#define GENG_GLINDEXBUFFER_REFERENCE_H

#include "opengl_includes.h"

#include "IndexBufferReference.h"

class GLGraphics;

class GLIndexBufferReference : public IndexBufferReference
{
public:
	GLIndexBufferReference(GLGraphics *graphics, int numIndices);
	virtual ~GLIndexBufferReference();
    virtual void activate();
	virtual void deactivate();
    virtual void destroy();
    void setId(unsigned int id) { bufferId = id; }
    void setIndexType(int type) { indexType = type; }
    int getIndexCount() { return indexCount; }
private:
    GLGraphics *graphicsObject;
    int *indices;
    int indexCount;
    int primitiveType;  // Triangles, quads, etc.
    int indexType;  // e.g., IndexBufferInfo::INT_16_BIT_INDICES
    unsigned int bufferId;
};

#endif
