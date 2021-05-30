#include "D3DGuiImageCreator.h"

#include "GuiImage.h"
#include "Vertex4.h"
#include "VertexBufferInfo.h"
#include "VertexBufferReference.h"
#include "IndexBufferInfo.h"
#include "IndexBufferReference.h"
#include "GraphicsSystem.h"
#include "Graphics.h"

GuiImage *D3DGuiImageCreator::createGuiImage(GuiImageInfo *info)
{
    GuiImage *image = new GuiImage(info->getOrientation(), info->getOffSetX(), info->getOffSetY(), info->getImageFileName());

    float maxU = image->getTextureCoordUMax();
    float maxV = image->getTextureCoordVMax();

    unsigned short indices[6] = { 0, 1, 2, 2, 3, 0 };
    Vertex4 *vertices = new Vertex4[4];

    vertices[0].x = static_cast<float>(image->getScreenCoords().left);
    vertices[0].y = static_cast<float>(image->getScreenCoords().bottom);
    vertices[0].z = 0;
    vertices[0].rhw = 1.0f;
    vertices[0].u = 0;
    vertices[0].v = maxV;

    vertices[1].x = static_cast<float>(image->getScreenCoords().left);
    vertices[1].y = static_cast<float>(image->getScreenCoords().top);
    vertices[1].z = 0;
    vertices[1].rhw = 1.0f;
    vertices[1].u = 0;
    vertices[1].v = 0;
    
    vertices[2].x = static_cast<float>(image->getScreenCoords().right);
    vertices[2].y = static_cast<float>(image->getScreenCoords().top);
    vertices[2].z = 0;
    vertices[2].rhw = 1.0f;
    vertices[2].u = maxU;
    vertices[2].v = 0;

    vertices[3].x = static_cast<float>(image->getScreenCoords().right);
    vertices[3].y = static_cast<float>(image->getScreenCoords().bottom);
    vertices[3].z = 0;
    vertices[3].rhw = 1.0f;
    vertices[3].u = maxU;
    vertices[3].v = maxV;

    VertexBufferInfo *vbInfo = new VertexBufferInfo(VertexBufferInfo::VERTEX4, 4);
    vbInfo->setVertexData(vertices);
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    VertexBufferReference *vbRef = graphics->createVertexBuffer(vbInfo);
    image->setVertexBufferReference(vbRef);

    IndexBufferInfo *ibInfo = new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES, 6);
    ibInfo->setIndexData(indices);
    IndexBufferReference *ibRef = graphics->createIndexBuffer(ibInfo);
    image->setIndexBufferReference(ibRef);

    delete vbInfo;
    delete ibInfo;

    return image;
}