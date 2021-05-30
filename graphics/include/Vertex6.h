#ifndef GENG_VERTEX6_H
#define GENG_VERTEX6_H

// For use in Direct3D.
// Corresponds to FVF format D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
struct Vertex6
{
    float x, y, z;  // positions
    unsigned int color;  // color (Window's type is DWORD)
    float tu, tv; // texture coords
};

#endif