#ifndef GENG_GUI_GLYPH_H
#define GENG_GUI_GLYPH_H

#include <map>

struct GuiGlyph
{
    /// The left position of the character image in the texture.
	int x;

    /// The top position of the character image in the texture.
    int y;

    /// The width of the character image in the texture.
    int width;

    /// The height of the character image in the texture.
    int height;

    /// How much the current position should be offset when copying the image from the texture to the screen.
    int offsetX;

    /// How much the current position should be offset when copying the image from the texture to the screen.
    int offsetY;

    /// How much the current position should be advanced after drawing the character.
	int xAdvance;

    /// The texture page where the character image is found. (Not currently used.)
	int page;

    std::map<char, int> kernings;

	GuiGlyph() : x(0), y(0), width(0), height(0), offsetX(0), offsetY(0), xAdvance(0), page(0) { }
};

#endif
