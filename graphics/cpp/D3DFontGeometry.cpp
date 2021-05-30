#include "D3DFontGeometry.h"

#include "GuiFont.h"
#include "GuiGlyph.h"
#include "GraphicsSystem.h"
#include "Graphics.h"
#include "IndexBufferInfo.h"
#include "Vertex4.h"
#include "VertexBufferInfo.h"
#include "ScreenPosition.h"

void D3DFontGeometry::initialize(int stringLength)
{
    // This could be refactored with GLFontGeometry
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

    // This is were things differ from GLFontGeometry
    // Use Vertex4 for the vertex buffer
    
    int numberOfVertices = maxStringLength * 4;
    Vertex4 *vertices = new Vertex4[numberOfVertices];
   
    VertexBufferInfo *vInfo = 
        new VertexBufferInfo(VertexBufferInfo::VERTEX4,
                             numberOfVertices);
    vInfo->setVertexData(vertices);

    vertexBuffer = graphics->createVertexBuffer(vInfo);

    delete [] vertices;
    delete vInfo;
}

void D3DFontGeometry::updateBufferObjects()
{   
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics(); // TODO: maybe pass as argument?
    GuiGlyph *glyphs = guiFont->getGlyphs();
    int currentScreenLeft = screenX;

    int numberOfVertices = textString.length() * 4;

    Vertex4 *vertices = new Vertex4[numberOfVertices];

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

        float textureWidthFloat = guiFont->getTextureWidthFloat() * .999f;  // TODO: This is a hack to get the correct view of character width.
        float textureHeightFloat = guiFont->getTextureHeightFloat() * .999f; // TODO: This is a hack to get the vertical center correct.

        float textureLeft = glyphs[c].x / textureWidthFloat;
        float textureTop = glyphs[c].y / textureHeightFloat;
        float textureRight = (glyphs[c].x + glyphs[c].width) / textureWidthFloat;
        float textureBottom = (glyphs[c].y + glyphs[c].height) / textureHeightFloat;

        int screenLeft = currentScreenLeft + glyphs[c].offsetX + kerning;
        int screenRight = screenLeft + glyphs[c].width;
        int screenTop = screenY + glyphs[c].offsetY; // Only the sign here differs from OpenGL version 
        int screenBottom = screenTop + glyphs[c].height;  // Only the sign here differs from OpenGL version.

        Vertex4 vertex1;
        vertex1.x = static_cast<float>(screenLeft);
        vertex1.y = static_cast<float>(screenTop);
        vertex1.z = 0;
        vertex1.rhw = 1.0f;
        vertex1.u = textureLeft;
        vertex1.v = textureTop;
        vertices[vertexIndex++] = vertex1;

        Vertex4 vertex2;
        vertex2.x = static_cast<float>(screenRight);
        vertex2.y = static_cast<float>(screenTop);
        vertex2.z = 0;
        vertex2.rhw = 1.0f;
        vertex2.u = textureRight;
        vertex2.v = textureTop;
        vertices[vertexIndex++] = vertex2;

        Vertex4 vertex3;
        vertex3.x = static_cast<float>(screenRight);
        vertex3.y = static_cast<float>(screenBottom);
        vertex3.z = 0;
        vertex3.rhw = 1.0f;
        vertex3.u = textureRight;
        vertex3.v = textureBottom;
        vertices[vertexIndex++] = vertex3;

        Vertex4 vertex4;
        vertex4.x = static_cast<float>(screenLeft);
        vertex4.y = static_cast<float>(screenBottom);
        vertex4.z = 0;
        vertex4.rhw = 1.0f;
        vertex4.u = textureLeft;
        vertex4.v = textureBottom;
        vertices[vertexIndex++] = vertex4;

        currentScreenLeft += glyphs[c].xAdvance + kerning;
    }
    vertexBuffer->writeData(vertices, numberOfVertices * sizeof(Vertex4));
    delete [] vertices;

    currentIndexCount = textString.size() * 6;
}