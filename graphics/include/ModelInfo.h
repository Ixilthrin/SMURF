#ifndef GENG_MODEL_INFO_H
#define GENG_MODEL_INFO_H

#include <string>
#include "KeyFrameAnimation.h"

/**
 *  Contains information needed to create a Model
 */
class ModelInfo
{
public:
    ModelInfo();
    ~ModelInfo();
    void setFileName(std::string name) { fileName = name; }
    std::string getFileName() { return fileName; }

    // These are temporary and will be moved into a Mesh object
    // They are public due to their temporary nature.
    float *vertexPositions;  // contains an x, y, and z values for each vertex
    int vertexCount;
    unsigned short *indices;  // 3 for each triangle
    int indexCount;
    bool hasNormals;
    float *normals;
    bool hasColorValues;
    float *colorValues;  // contains red, blue, and green values for each vertex.
    int colorValueCount;
    bool hasTexture;
    float *textureCoords;
    std::string textureFile;
    bool hasSecondTexture;
    float *textureCoords2;
    std::string textureFile2;  // set to "[none]" if no file is used, such as using
                               // texure coordinates to pass vertex index.
    bool hasThirdTexture;
    float *textureCoords3;
    std::string textureFile3;  // set to "[none]" if no file is used, such as using
                               // texure coordinates for bone weights or bone indices

    float *boneIndices;
    float *boneWeights;

    KeyframeAnimation *animation;
    
private:
    std::string fileName;

};

#endif
