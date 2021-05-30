#ifndef GENG_VERTEX4_H
#define GENG_VERTEX4_H

// Represents a HUD vertex in Direct3D
struct Vertex4
{
    float x, y;  // screen positions
    float z;  // should be 0
    float rhw;  // reciprocal homogeneous W (usually == 1)
    float u, v;  // Texture coordinates
};
// The FVF is D3DFVF_XYZRHW | D3DFVF_TEX1

#endif
