#ifndef GENG_VERTEX3_H
#define GENG_VERTEX3_H

// For use in Direct3D.
// Corresponds to FVF format D3DFVF_XYZ | D3DFVF_DIFFUSE
struct Vertex3
{
    float x, y, z;  // positions
    unsigned int color;  // color (Window's type is DWORD)
};

#endif