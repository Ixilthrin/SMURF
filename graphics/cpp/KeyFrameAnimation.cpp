#include "KeyFrameAnimation.h"

KeyframeAnimation::KeyframeAnimation(int theNumberOfVertices)
: numberOfKeyframes(0), numberOfVertices(theNumberOfVertices)
{
    skinWeights = new SkinWeights(theNumberOfVertices);
}

int KeyframeAnimation::getIndexForBone(std::string boneName)
{
    int size = bones.size();
    for (int i = 0; i < size; ++i)
    {
        Bone *currentBone = bones[i];
        if (currentBone->getName() == boneName)
        {
            return currentBone->getIndex();
        }
    }
    return -1;
}

/**
 *  Search through the vector of bones for a bone
 *  with @boneName and return the bone. 
 *  If not found, return 0.
 */
Bone *KeyframeAnimation::getBoneByName(std::string boneName)
{
    int nBones = bones.size();
    for (int i = 0; i < nBones; ++i)
    {
        if (bones[i]->getName() == boneName)
        {
            return bones[i];
        }
    }
    return 0;
}

Bone *KeyframeAnimation::getBoneByIndex(int index)
{
    int nBones = bones.size();
    for (int i = 0; i < nBones; ++i)
    {
        if (bones[i]->getIndex() == index)
        {
            return bones[i];
        }
    }
    return 0;
}

BoneAnimation *KeyframeAnimation::getBoneAnimationByBoneIndex(int boneIndex)
{
    int nBonesAnimations = boneAnimations.size();
    for (int i = 0; i < nBonesAnimations; ++i)
    {
        if (boneAnimations[i]->getBoneIndex() == boneIndex)
        {
            return boneAnimations[i];
        }
    }
    return 0;
}

BoneAnimation *KeyframeAnimation::getParentBoneAnimation(BoneAnimation *boneAnimation)
{
    if (!boneAnimation)
        return 0;
    int boneIndex = boneAnimation->getBoneIndex();
    Bone *bone = getBoneByIndex(boneIndex);
    if (!bone)
        return 0;
    std::string parentName = bone->getParentName();
    Bone *parentBone = getBoneByName(parentName);
    if (!parentBone)
        return 0;
    int parentIndex = parentBone->getIndex();
    return getBoneAnimationByBoneIndex(parentIndex);
}
