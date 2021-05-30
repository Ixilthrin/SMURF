#include <sstream>
#include "utXMLParser.h"
#include "util.h"
#include "FontGeometry.h"
#include "GuiFont.h"
#include "GuiGlyph.h"
#include "GuiTexture.h"
#include "Graphics.h"
#include "GraphicsSystem.h"

std::map<std::string, GuiFont *> GuiFont::fontNameMap;

GuiFont * GuiFont::create(const std::string & fontName)
{
    GuiFont * guiFont = 0;
    if (fontNameMap.count(fontName) > 0)
    {
        guiFont = fontNameMap[fontName];
    }
    else
    {
        guiFont = new GuiFont(fontName);
        fontNameMap[fontName] = guiFont;
    }
    guiFont->acquireReference();
    return guiFont;
}

GuiFont::GuiFont(const std::string & fontName) : referenceCount(1), fontName(fontName)
{
    std::string fileName = fontName + "_0.tga";
    guiTexture = GuiTexture::load(fileName);
    textureWidthFloat = static_cast<float>(guiTexture->getWidth());
    textureHeightFloat = static_cast<float>(guiTexture->getHeight());

    // Parse document and get root node.
    XMLNode rootNode = XMLNode::openFileHelper((fontName + ".fnt").c_str(), "font");

    // Process common element.
    XMLNode commonNode = rootNode.getChildNode("common");

    // Extract lineHeight.
    std::string lineHeightString = commonNode.getAttribute("lineHeight");
    std::stringstream lineHeightStream(lineHeightString);
    if ((lineHeightStream >> lineHeight).fail())
    {
        fatalError("Failed to convert " + lineHeightString + " to int in file " + fileName);
    }

    // Extract base.
    std::string baseString = commonNode.getAttribute("base");
    std::stringstream baseStream(baseString);
    if ((baseStream >> base).fail())
    {
        fatalError("Failed to convert " + baseString + " to int in file " + fileName);
    }

    // Extract pages.
    std::string pagesString = commonNode.getAttribute("pages");
    std::stringstream pagesStream(pagesString);
    if ((pagesStream >> pages).fail())
    {
        fatalError("Failed to convert " + pagesString + " to int in file " + fileName);
    }
    if (pages != 1)
    {
        fatalError(
            "Glyph image files in " + 
            fontName + " is " + pagesString + 
            ". System does not support Angel Code Bit Mapped Fonts with more than 1 page.");
    }

    // Process chars element.
    XMLNode charsNode = rootNode.getChildNode("chars");

    // Extract each char within the chars element.
    int numberOfCharElements = charsNode.nChildNode("char");
    for (int i = 0; i < numberOfCharElements; ++i)
    {
        XMLNode charNode = charsNode.getChildNode("char", i);

        // Extract id.
        std::string idString = charNode.getAttribute("id");
        std::stringstream idStream(idString);
        int id;
        if ((idStream >> id).fail())
        {
            fatalError("Failed to convert " + idString + " to int in file " + fileName);
        }
        if (id > 256)
        {
            fatalError(
                fontName + " contains a char with id = " + idString + 
                ". The system does not support Angel Code bit mapped fonts with more than 256 chars," +
                "but this is easily changed.");
        }
        GuiGlyph & glyph = glyphs[id];

        // Extract x.
        std::string xString = charNode.getAttribute("x");
        std::stringstream xStream(xString);        
        if ((xStream >> glyph.x).fail())
        {
            fatalError("Failed to convert " + xString + " to int in file " + fileName);
        }

        // Extract y.
        std::string yString = charNode.getAttribute("y");
        std::stringstream yStream(yString); 
        if ((yStream >> glyph.y).fail())
        {
            fatalError("Failed to convert " + yString + " to int in file " + fileName);
        }

        // Extract width.
        std::string widthString = charNode.getAttribute("width");
        std::stringstream widthStream(widthString);        
        if ((widthStream >> glyph.width).fail())
        {
            fatalError("Failed to convert " + widthString + " to int in file " + fileName);
        }

        // Extract height.
        std::string heightString = charNode.getAttribute("height");
        std::stringstream heightStream(heightString); 
        if ((heightStream >> glyph.height).fail())
        {
            fatalError("Failed to convert " + heightString + " to int in file " + fileName);
        }

        // Extract xoffset.
        std::string offsetXString = charNode.getAttribute("xoffset");
        std::stringstream offsetXStream(offsetXString);
        if ((offsetXStream >> glyph.offsetX).fail())
        {
            fatalError("Failed to convert " + offsetXString + " to int in file " + fileName);
        }

        // Extract yoffset.
        std::string offsetYString = charNode.getAttribute("yoffset");
        std::stringstream offsetYStream(offsetYString); 
        if ((offsetYStream >> glyph.offsetY).fail())
        {
            fatalError("Failed to convert " + offsetYString + " to int in file " + fileName);
        }

        // Extract xadvance.
        std::string xAdvanceString = charNode.getAttribute("xadvance");
        std::stringstream xAdvanceStream(xAdvanceString);        
        if ((xAdvanceStream >> glyph.xAdvance).fail())
        {
            fatalError("Failed to convert " + xAdvanceString + " to int in file " + fileName);
        }

        // Extract page.
        std::string pageString = charNode.getAttribute("page");
        std::stringstream pageStream(pageString); 
        if ((pageStream >> glyph.page).fail())
        {
            fatalError("Failed to convert " + pageString + " to int in file " + fileName);
        }  
    }
    
    // Process kernings element.
    XMLNode kerningsNode = rootNode.getChildNode("kernings");

    // Extract each kerning within the kernings element.
    int numberOfKerningElements = kerningsNode.nChildNode("kerning");
    for (int i = 0; i < numberOfKerningElements; ++i)
    {
        XMLNode kerningNode = kerningsNode.getChildNode("kerning", i);

        // Extract first.
        std::string firstString = kerningNode.getAttribute("first");
        std::stringstream firstStream(firstString);
        int firstId;
        if ((firstStream >> firstId).fail())
        {
            fatalError("Failed to convert " + firstString + " to int in file " + fileName);
        }

        // Extract second.
        std::string secondString = kerningNode.getAttribute("second");
        std::stringstream secondStream(secondString);
        int secondId;
        if ((secondStream >> secondId).fail())
        {
            fatalError("Failed to convert " + secondString + " to int in file " + fileName);
        }

        // Extract amount.
        std::string amountString = kerningNode.getAttribute("amount");
        std::stringstream amountStream(amountString);
        int amount;
        if ((amountStream >> amount).fail())
        {
            fatalError("Failed to convert " + amountString + " to int in file " + fileName);
        }

        GuiGlyph & leftGlyph = glyphs[firstId];
        leftGlyph.kernings[secondId] = amount;
    }
    //Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    //fontGeometry = graphics->createFontGeometry();
    //fontGeometry->setGuiFont(this);
    //fontGeometry->initialize(100); // TODO: string length hard-coded? Change this.
}

void GuiFont::acquireReference()
{
    ++referenceCount;
}

void GuiFont::releaseReference()
{
    --referenceCount;
    if (referenceCount == 0)
    {
        guiTexture->releaseReference();
        fontNameMap.erase(fontName);
        delete this;
        return;
    } 
    else if (referenceCount < 0)
    {
        fatalError("GuiFont::releaseReference called more times than GuiFont::acquireReference.");
    }
}

void GuiFont::computeDimensions(const std::string & text, int & width, int & height)
{
    height = lineHeight;
    width = 0;
    for (unsigned int i = 0; i < text.size(); ++i)
    {
        char c = text[i];
        GuiGlyph & glyph = glyphs[c];
        int kerning = 0;
        if (i > 0)
        {
            char leftc = text[i - 1];
            if (leftc >= 0 && glyph.kernings.count(leftc) > 0)
            {
                kerning = glyph.kernings[leftc];
            }
        }
        width += glyphs[c].xAdvance + kerning;
    }
}

FontGeometry *GuiFont::updateTextAndPosition(const std::string & textString, int x, int y)
{
    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    FontGeometry *fontGeometry = graphics->createFontGeometry();
    fontGeometry->setGuiFont(this);
    fontGeometry->initialize(100); // TODO: string length hard-coded? Change this.
    fontGeometry->updateTextAndPosition(textString, x, y);
    return fontGeometry;
}

void GuiFont::display(FontGeometry *fontGeometry)
{
    guiTexture->getTextureReference()->activate();
    fontGeometry->display();
    guiTexture->getTextureReference()->deactivate();
}
