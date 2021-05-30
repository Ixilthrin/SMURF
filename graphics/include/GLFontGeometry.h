#ifndef GENG_GL_FONT_GEOMETRY_H
#define GENG_GL_FONT_GEOMETRY_H

#include "FontGeometry.h"

class GLFontGeometry : public FontGeometry
{
public:
    virtual void initialize(int stringLength);
    virtual void updateBufferObjects();
};

#endif