#ifndef GENG_FONT_GEOMETRY_H
#define GENG_FONT_GEOMETRY_H

#include "IndexBufferReference.h"
#include "VertexBufferReference.h"
#include <string>

class GuiFont;

/**
 *  This class should be renamed to TextGeometry,
 *  since it represents the vertices and 
 *  texture coordinates of a specific text string.
 */
class FontGeometry
{
public:
    FontGeometry();
    void setGuiFont(GuiFont *font) { guiFont = font; }
    virtual void initialize(int stringLength) = 0;
    void initPosition(int x, int y) { screenX = x; screenY = y; }
    void updateTextAndPosition(const std::string & textString, int x, int y);
    void display();
    virtual void updateBufferObjects() = 0;
    void setIndexBufferReference(IndexBufferReference *ref) { indexBuffer = ref; }
    void setVertexBufferReference(VertexBufferReference *ref) { vertexBuffer = ref; }
protected:
    GuiFont *guiFont;
    int screenX;
    int screenY;
    std::string textString;
    int maxStringLength;
    IndexBufferReference *indexBuffer;
    VertexBufferReference *vertexBuffer;
    int currentIndexCount;  // represents current indices being used.
};

#endif