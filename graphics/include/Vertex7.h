#ifndef GENG_VERTEX_7_H
#define GENG_VERTEX_7_H

/**
 *  The initial purpose of this vertex format
 *  is for keyframe animation, although it
 *  can be used for other purposes.
 *  The second set of texture coordinates is
 *  used to pass the index of the vertex
 *  to be processed.  This is needed in the
 *  vertex shader (if used for animation).
 */
struct Vertex7
{
    float x, y, z;  // position
    unsigned int color;  // color (Window's type is DWORD)
    float nx, ny, nz;  // normal
    float u1, v1;  // texture coordinates
    float u2, v2;  // texture coordinates
};

#endif
