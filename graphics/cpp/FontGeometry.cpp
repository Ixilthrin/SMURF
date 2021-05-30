#include "FontGeometry.h"
#include "GraphicsSystem.h"
#include "Graphics.h"
#include "GuiFont.h"

// TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"

FontGeometry::FontGeometry()
: screenX(0), screenY(0)
{
    indexBuffer = 0;
    vertexBuffer = 0;
}

void FontGeometry::updateTextAndPosition(const std::string & textString, int x, int y)
{
    this->textString = textString;
    screenX = x;
    screenY = y;
    this->updateBufferObjects();
}

void FontGeometry::display()
{
    // Graphics object should be passed as an argument
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    graphics->disableMode(Graphics::CULLING);
    graphics->disableMode(Graphics::LIGHTING);
    indexBuffer->activate();
    vertexBuffer->activate(false);
    graphics->drawIndexedTriangles(currentIndexCount, 0);  // change 3 to currentIndexCount
    vertexBuffer->deactivate();
    indexBuffer->deactivate();
    
/*
    GuiGlyph *glyphs = guiFont->getGlyphs();
    int currentScreenLeft = screenX;
    for (unsigned int i = 0; i < textString.size(); ++i)
    {
        char c = textString[i];
        GuiGlyph & glyph = glyphs[c];

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

        glBegin(GL_QUADS);
        glTexCoord2f(textureLeft, textureBottom);
        glVertex2i(screenLeft, screenBottom); 
        glTexCoord2f(textureRight, textureBottom);
        glVertex2i(screenRight, screenBottom);
        glTexCoord2f(textureRight, textureTop);
        glVertex2i(screenRight, screenTop);
        glTexCoord2f(textureLeft, textureTop);
        glVertex2i(screenLeft, screenTop);
        glEnd();

        currentScreenLeft += glyphs[c].xAdvance + kerning;
    }
    */
    
}