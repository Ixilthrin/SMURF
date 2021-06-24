#ifndef GENG_COLOR_H
#define GENG_COLOR_H

struct Color
{
    float red;
    float green;
    float blue;
    float alpha;
    Color() : red(0), green(0), blue(0), alpha(0) { }
    Color(float r, float g, float b, float a) : red(r), green(g), blue(b), alpha(a) { }
};


#endif
