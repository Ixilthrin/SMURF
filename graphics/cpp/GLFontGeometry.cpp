#include "GLFontGeometry.h"

#include "GraphicsSystem.h"
#include "Graphics.h"
#include "GuiFont.h"
#include "GuiGlyph.h"
#include "IndexBufferInfo.h"
#include "Vertex1.h"
#include "VertexBufferInfo.h"

void GLFontGeometry::initialize(int stringLength)
{
    // This could be refactored with D3DFontGeometry
    // Just want to make sure winding order for
    // the triangles will be the same

    maxStringLength = stringLength;

    // Initialize the index buffer.
    // This should be constant, base on the string
    // length maximum and assuming each character
    // is associated with a quad = 2 triangles.

    // These indices are in counter-clockwise rotation. <-- is this what we want?

    currentIndexCount = 6 * maxStringLength;

    // Currently only 16 bit indices are supported
    unsigned short *indices = new unsigned short[currentIndexCount];

    int numberOfQuads = maxStringLength;
    int index = 0;
    int baseIndex = 0;
    for (int quad = 0; quad < numberOfQuads; quad++)
    {
        baseIndex = quad * 4;
        indices[index++] = 0 + baseIndex;
        indices[index++] = 1 + baseIndex;
        indices[index++] = 2 + baseIndex;
        indices[index++] = 3 + baseIndex;
        indices[index++] = 0 + baseIndex;
        indices[index++] = 2 + baseIndex;
    }

    IndexBufferInfo *iInfo = 
        new IndexBufferInfo(IndexBufferInfo::INT_16_BIT_INDICES,
                            currentIndexCount);
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    iInfo->setIndexData(indices);
    indexBuffer = graphics->createIndexBuffer(iInfo);

    delete [] indices;
    delete iInfo;

    // This is were things differ from D3DFontGeometry
    // Use Vertex1 for the vertex buffer

    int numberOfVertices = maxStringLength * 4;
    Vertex1 *vertices = new Vertex1[numberOfVertices];
   
    VertexBufferInfo *vInfo = 
        new VertexBufferInfo(VertexBufferInfo::VERTEX1,
                             numberOfVertices);
    vInfo->setVertexData(vertices);

    vertexBuffer = graphics->createVertexBuffer(vInfo);

    delete [] vertices;
    delete vInfo;

}

void GLFontGeometry::updateBufferObjects()
{    
    GuiGlyph *glyphs = guiFont->getGlyphs();
    int currentScreenLeft = screenX;

    int numberOfVertices = textString.length() * 4;  // Each character encapsulated in a quad

    Vertex1 *vertices = new Vertex1[numberOfVertices];

    // Vertices will be added to the vertex buffer starting at index 0.
    int vertexIndex = 0;

    for (unsigned int i = 0; i < textString.size(); ++i)
    {
        char c = textString[i];
        GuiGlyph glyph = glyphs[c];

        int kerning = 0;
        if (i > 0)
        {
            char leftc = textString[i - 1];
            if (leftc >= 0 && glyph.kernings.count(leftc) > 0)
            {
                kerning = glyph.kernings[leftc];
            }
        }

        float textureWidthFloat = guiFont->getTextureWidthFloat();
        float textureHeightFloat = guiFont->getTextureHeightFloat();

        float textureLeft = glyphs[c].x / textureWidthFloat;
        float textureTop = glyphs[c].y / textureHeightFloat;
        float textureRight = (glyphs[c].x + glyphs[c].width) / textureWidthFloat;
        float textureBottom = (glyphs[c].y + glyphs[c].height) / textureHeightFloat;

        int screenLeft = currentScreenLeft + glyphs[c].offsetX + kerning;
        int screenRight = screenLeft + glyphs[c].width;
        int screenTop = screenY - glyphs[c].offsetY;
        int screenBottom = screenTop - glyphs[c].height;

        Vertex1 vertex1;
        vertex1.x = static_cast<float>(screenLeft);
        vertex1.y = static_cast<float>(screenBottom);
        vertex1.z = 0;
        vertex1.u = textureLeft;
        vertex1.v = textureBottom;
        vertices[vertexIndex++] = vertex1;

        Vertex1 vertex2;
        vertex2.x = static_cast<float>(screenRight);
        vertex2.y = static_cast<float>(screenBottom);
        vertex2.z = 0;
        vertex2.u = textureRight;
        vertex2.v = textureBottom;
        vertices[vertexIndex++] = vertex2;

        Vertex1 vertex3;
        vertex3.x = static_cast<float>(screenRight);
        vertex3.y = static_cast<float>(screenTop);
        vertex3.z = 0;
        vertex3.u = textureRight;
        vertex3.v = textureTop;
        vertices[vertexIndex++] = vertex3;

        Vertex1 vertex4;
        vertex4.x = static_cast<float>(screenLeft);
        vertex4.y = static_cast<float>(screenTop);
        vertex4.z = 0;
        vertex4.u = textureLeft;
        vertex4.v = textureTop;
        vertices[vertexIndex++] = vertex4;

        currentScreenLeft += glyphs[c].xAdvance + kerning;
    }
    vertexBuffer->writeData(vertices, numberOfVertices * sizeof(Vertex1));
    delete [] vertices;

    currentIndexCount = textString.size() * 6;
}