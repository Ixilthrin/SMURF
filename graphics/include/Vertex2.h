#ifndef GENG_VERTEX2_H
#define GENG_VERTEX2_H

// For use in OpenGL.  Direct3D does not support this format.
struct Vertex2
{
    float x, y, z;  // positions
    float r, g, b, a;  // color
};

#endif
