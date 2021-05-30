#ifndef GENG_MATRIX_ARRAY_H
#define GENG_MATRIX_ARRAY_H

#include <stdio.h>

/** 
 *  Maybe poorly named.  This is a 4x4 matrix encapsulated as an array of floats,
 *  not an array of matrices.
 *  Used for convenience.
 *  Wrapper around a float array that
 *  represents a 4 x 4 matrix.
 */
struct MatrixArray
{
    MatrixArray()
    {
        for (int i = 0; i < 16; ++i)
        {
            transform[i] = 0;
        }
        transform[0] = 1.0f;
        transform[5] = 1.0f;
        transform[10] = 1.0f;
        transform[15] = 1.0f;
    }
    float transform[16];
    void setTransformRowMajor(float t[16])
    {
        for (int i = 0; i < 16; ++i)
        {
            transform[i] = t[i];
        }
    }
    void setTransformColumnMajor(float t[16])
    {
        transform[0] = t[0];
        transform[1] = t[4];
        transform[2] = t[8];
        transform[3] = t[12];
        transform[4] = t[1];
        transform[5] = t[5];
        transform[6] = t[9];
        transform[7] = t[13];
        transform[8] = t[2];
        transform[9] = t[6];
        transform[10] = t[10];
        transform[11] = t[14];
        transform[12] = t[3];
        transform[13] = t[7];
        transform[14] = t[11];
        transform[15] = t[15];
    }
    void setValue(int index, float value) { transform[index] = value; }
    static MatrixArray product(MatrixArray left, MatrixArray right)
    {
        MatrixArray result;
        for (int i = 0; i < 16; ++i)
        {
            int rowStartIndex = ((int) (i / 4)) * 4;
            int columnStartIndex = i % 4;
            result.transform[i] = left.transform[rowStartIndex] * right.transform[columnStartIndex]
                      + left.transform[rowStartIndex + 1] * right.transform[columnStartIndex + 4]
                      + left.transform[rowStartIndex + 2] * right.transform[columnStartIndex + 8]
                      + left.transform[rowStartIndex + 3] * right.transform[columnStartIndex + 12];
        }
        return result;
    }
    void print()
    {
        for (int i = 0; i < 16; ++i)
        {
            printf("%f  ", transform[i]);
            if (i % 4 == 3)
                printf("\n");
        }
    }
};

#endif