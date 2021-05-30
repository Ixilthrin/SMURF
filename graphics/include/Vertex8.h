#ifndef GENG_VERTEX_8_H
#define GENG_VERTEX_8_H

/**
 *  The initial purpose of this vertex format
 *  is for keyframe animation, although it
 *  can be used for other purposes.
 */
struct Vertex8
{
    float x, y, z;  // position
    //unsigned int color;  // color (Window's type is DWORD)
    float nx, ny, nz;  // normal
    float u1, v1; // texture coordinates
    float u2, v2; // texture coordinates (bone indices)
    float u3, v3; // texture coordinates (bone weights)
};

#endif
