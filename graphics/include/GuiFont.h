#ifndef GENG_GUI_FONT_H
#define GENG_GUI_FONT_H

#include <string>
#include <map>
#include "GuiGlyph.h"
#include "SelfDeletingResource.h"
#include "FontGeometry.h"
class GuiTexture;

/**
\brief Bitmapped fonts loaded from AngleCode's Bitmap Font Generator. 

This class assumes that the bitmapped fonts are generated from AngleCode's Bitmap Font Generator.
When exporting fonts from this program, all glyphs must be stored in a single Targa image file.
Also, when running the Bitmap Font Generator, export fonts using XML and 4 byte pixel depth.

\see gengCreateFont
*/
class GuiFont : public SelfDeletingResource
{
public:
    static GuiFont * create(const std::string & fontName);
    virtual void acquireReference();
    virtual void releaseReference();
    FontGeometry *updateTextAndPosition(const std::string & text, int x, int y);
    void display(FontGeometry *fontGeometry);
    void computeDimensions(const std::string & text, int & width, int & height);
    GuiGlyph *getGlyphs() { return glyphs; }
    float getTextureWidthFloat() { return textureWidthFloat; }
    float getTextureHeightFloat() { return textureHeightFloat; }

private:
    static std::map<std::string, GuiFont *> fontNameMap;

    GuiFont(const std::string & fontName);
    ~GuiFont() { }

    std::string fontName;
    int referenceCount;
    GuiTexture * guiTexture;
    float textureWidthFloat;
    float textureHeightFloat;
	int lineHeight;

    //FontGeometry *fontGeometry;

    /// The number of pixels from the absolute top of the line to the base of the characters.
	int base;  // not sure what to use this for

    /// Number of separate image files.  Assumed to be 1.
	int pages; // not used

    //std::map<char, Glyph> glyphMap;
	GuiGlyph glyphs[256];  // should turn this into a map to conserve space.
};

#endif
