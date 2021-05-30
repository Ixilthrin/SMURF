#ifndef GENG_D3D_FONT_GEOMETRY_H
#define GENG_D3D_FONT_GEOMETRY_H

#include "FontGeometry.h"

class D3DFontGeometry : public FontGeometry
{
public:
    virtual void initialize(int stringLength);
    virtual void updateBufferObjects();
};

#endif