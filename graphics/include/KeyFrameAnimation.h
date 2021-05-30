#ifndef GENG_KEY_FRAME_ANIMATION_H
#define GENG_KEY_FRAME_ANIMATION_H

#include "MatrixArray.h"

#include <string>
#include <map>
#include <vector>

struct BoneIndices
{
    int bone1Index;
    int bone2Index;
    int bone3Index;
    int bone4Index;
};

struct BoneWeights
{
    float bone1Weight;
    float bone2Weight;
    float bone3Weight;
    float bone4Weight;
};

class VertexBoneWeights
{
public:
    VertexBoneWeights() : currentIndex(0) 
    {
        for (int i = 0; i < 4; ++i)
        {
            boneIndices[i] = 0.0f;
            boneWeights[i] = 0.0f;
        }
    }
    void addBoneWeight(float boneIndex, float boneWeight)
    {
        if (currentIndex >= 4)
        {
            return; // Too many bones, ignore remaining
        }
        boneIndices[currentIndex] = boneIndex;
        boneWeights[currentIndex] = boneWeight;
        currentIndex++;
    }
    float *getBoneIndices() { return boneIndices; }
    float *getBoneWeights() { return boneWeights; }
private:
    int currentIndex;
    float boneIndices[4];
    float boneWeights[4];
};

/**
 *  Purpose: Maps vertices to bone indices and weights
 */
class SkinWeights
{
public:
    SkinWeights(int theNumberOfVertices) : numberOfVertices(theNumberOfVertices)
    {
        weights = new VertexBoneWeights[numberOfVertices];
    }
    ~SkinWeights()
    {
        delete [] weights;
    }
    void addVertexBoneWeight(int vertexIndex, float boneIndex, float boneWeight)
    {
        weights[vertexIndex].addBoneWeight(boneIndex, boneWeight);
    }
    int getNumberOfVertices() { return numberOfVertices; }
    VertexBoneWeights getBoneWeights(int vertexIndex) { return weights[vertexIndex]; }
private:
    int numberOfVertices;

    // An array with numberOfVertices values
    VertexBoneWeights *weights;
};

class Bone
{
public:
    Bone(int theIndex) : index(theIndex) {}
    int getIndex() { return index; }
    std::string getName() { return name; }
    void setName(std::string theName) { name = theName; }
    void setFrameTransform(MatrixArray matrix) { frameTransform = matrix; }
    MatrixArray getFrameTransform() { return frameTransform; }
    void setOffsetTransform(MatrixArray matrix) { offsetTransform = matrix; }
    MatrixArray getOffsetTransform() { return offsetTransform; }
    void setParentName(std::string name) { parentName = name; }
    std::string getParentName() { return parentName; }
private:
    int index;
    std::string name;
    std::string parentName;
    MatrixArray frameTransform;
    MatrixArray offsetTransform;
};


/** 
 *  Keyframes for a single bone in the animation
 */
class BoneAnimation
{
public:
    BoneAnimation(int theBoneIndex) : boneIndex(theBoneIndex), transforms(0), parent(0) {}
    void addBoneTransform(MatrixArray transform) { transforms.push_back(transform); }
    int getBoneIndex() { return boneIndex; }
    int getNumberOfTransforms() { return transforms.size(); }
    MatrixArray getTransform(int index) { return transforms[index]; }
private:
    int boneIndex;  // The Bone associated with this BoneAnimation
    BoneAnimation *parent;

    // Array of AnimationTransforms, one for each keyframe
    // Each transform transforms an associated vertex to its new position
    std::vector<MatrixArray>  transforms;
};

/**
 *  A single animation of a 3D model
 */
class KeyframeAnimation
{
public:
    KeyframeAnimation(int theNumberOfVertices);
    void addBone(Bone *bone) { bones.push_back(bone); }
    void addBoneAnimation(BoneAnimation *anim) { boneAnimations.push_back(anim); }
    SkinWeights *getSkinWeights() { return skinWeights; }
    void setNumberOfKeyframes(int nKeyframes) { numberOfKeyframes = nKeyframes; }
    int getIndexForBone(std::string boneName);  // Return -1 if no bone with given name
    int getNumberOfKeyframes() { return numberOfKeyframes; }
    int getNumberOfBones() { return bones.size(); }
    Bone *getBoneByName(std::string boneName);
    Bone *getBoneByIndex(int index);
    BoneAnimation *getBoneAnimationByBoneIndex(int boneIndex);
    BoneAnimation *getParentBoneAnimation(BoneAnimation *boneAnimation);
private:
    std::string name;
    int numberOfKeyframes;
    std::vector<Bone *> bones;
    std::vector<BoneAnimation *> boneAnimations; // vector of BoneAnimations, one for each bone.
    SkinWeights *skinWeights;
    int numberOfVertices;
};

#endif
