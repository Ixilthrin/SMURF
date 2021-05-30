#ifndef GENG_MESH_H
#define GENG_MESH_H

#include "Graphics.h"
#include "IndexBufferReference.h"
#include "TextureReference.h"
#include "VertexBufferReference.h"

/**
 *  Encapsulates a vertex buffer and an index buffer.
 */
class Mesh
{
public:
    Mesh();
    ~Mesh();
    void draw(Graphics *graphics, bool picking);
    void setIndexBuffer(IndexBufferReference *ref) { indexBuffer = ref; }
    void setVertexBuffer(VertexBufferReference *ref) { vertexBuffer = ref; }
    VertexBufferReference *getVertexBuffer() { return vertexBuffer; }
    IndexBufferReference *getIndexBuffer() { return indexBuffer; }
private:
    IndexBufferReference *indexBuffer;
    VertexBufferReference *vertexBuffer;
};

#endif