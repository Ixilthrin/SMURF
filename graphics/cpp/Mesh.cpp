#include "Mesh.h"

Mesh::Mesh()
: indexBuffer(0), vertexBuffer(0)
{
}

Mesh::~Mesh()
{
    if (indexBuffer)
    {
        delete indexBuffer;
        indexBuffer = 0;
    }
    if (vertexBuffer)
    {
        delete vertexBuffer;
        vertexBuffer = 0;
    }
}

void Mesh::draw(Graphics *graphics, bool picking)
{
    indexBuffer->activate();
    vertexBuffer->activate(picking);
    int indexCount = indexBuffer->getIndexCount();
    graphics->drawIndexedTriangles(indexCount, 0);
    vertexBuffer->deactivate();
    indexBuffer->deactivate();
}