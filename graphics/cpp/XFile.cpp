#include "XFile.h"
#include <iostream>
#include "util.h"
#include <stdlib.h>


XFileObjectTypeCollector::XFileObjectTypeCollector(std::vector<XFileObject *> theRootObjects)
{
    rootObjects = theRootObjects;
}

std::vector<XFileObject *> XFileObjectTypeCollector::getMatchingObjects(std::string theType)
{
    matchingObjects.clear();
    type = theType;
    int size = rootObjects.size();
    for (int i = 0; i < size; ++i)
    {
        visit(rootObjects[i]);
    }
    return matchingObjects;
}

void XFileObjectTypeCollector::visit(XFileObject *object)
{
    if (object->type == type)
        matchingObjects.push_back(object);
    std::vector<XFileObject *> children = object->children;
    int size = children.size();
    for (int i = 0; i < size; ++i)
    {
        visit(children[i]);
    }
}

XFile::XFile(std::string theFileName)
: file(fileName.c_str(), std::ios_base::in),
  objectStack(), rootObjects(), fileName(theFileName)
{
    char c;
    while (true)
    {
        c = (char) file.peek();
        // Log some information
        if (file.eof())
        {
            break;
        }
        if (c < 0)
        {
            fatalError(theFileName + " does not exist or is corrupted");
        }
        if (isAlpha(c))
        {
            std::string str = readString();
            //std::cout << "string = " << str << std::endl;

            if (str == std::string("template"))
            {
                // Ignore template nodes (supported implicitly)
                while (c != '}' && !file.eof())
                {
                    c = file.get();
                }
                if (c == '}' && !file.eof())
                {
                    file.get();
                }
            }
            else if (str == std::string("Frame") || str == std::string("Material")
                || str == std::string("AnimationSet"))
            {
                // There will be a corresponding name with this element so move
                // ahead to the next string position (ignore whitespace)
                c = file.peek();
                while (c == ' ' || c == '\n' || c == '\t' || c == '\r')
                {
                    c = file.get();
                    c = file.peek();
                }
                XFileObject *namedObject = new XFileObject();
                namedObject->type = str;
                namedObject->name = readString();
                if (objectStack.empty())
                {
                    rootObjects.push_back(namedObject);
                    namedObject->parent = 0;
                }
                else
                {
                    XFileObject *parent = objectStack.top();
                    if (parent)
                    {
                        parent->children.push_back(namedObject);
                        namedObject->parent = parent;
                    }
                }
                objectStack.push(namedObject);

                // Read until the beginning of the block
                while (c != '{' && !file.eof())
                {
                    c = file.get();
                }
            }
            else if (str == std::string("Mesh") || str == std::string("FrameTransformMatrix")
                || str == std::string("MeshMaterialList") || str == std::string("MeshNormals")
                || str == std::string("MeshTextureCoords") || str == std::string("TextureFilename")
                || str == std::string("MeshVertexColors")
                || str == std::string("XSkinMeshHeader") || str == std::string("SkinWeights")
                || str == std::string("Animation") || str == std::string("AnimationKey")
                || str == std::string("AnimTicksPerSecond"))
            {
                XFileObject *object = new XFileObject();
                object->type = str;
                if (objectStack.empty())
                {
                    rootObjects.push_back(object);
                    object->parent = 0;
                }
                else
                {
                    XFileObject *parent = objectStack.top();
                    if (parent)
                    {
                        parent->children.push_back(object);

                        object->parent = parent;
                    }
                }
                objectStack.push(object);

                // Read until the beginning of the block
                while (c != '{' && !file.eof())
                {
                    c = file.get();
                }
            }
            else
            {
                if (!objectStack.empty())
                {
                    XFileObject *parent = objectStack.top();
                    if (parent)
                    {
                        parent->strings.push_back(str);
                    }
                }
            }
        }
        else if (isNumeric(c) || c == '-' || c == '.')
        {
            float num = readNumber();
            //std::cout << "number = " << num << std::endl;
            if (!objectStack.empty())
            {
                XFileObject *context = objectStack.top();
                if (context)
                {
                    context->numbers.push_back(num);
                }
            }
        }
        else if (c == '"')
        {
            file.get();  // Remove the beginning '"'
            std::string str = readString();
            XFileObject *context = objectStack.top();
            if (context)
            {
                context->strings.push_back(str);
            }
            file.get();  // Remove the ending '"'.  TODO:  Error if not found
        }
        else if (c == '}')
        {
            objectStack.pop();
            file.get();  // Important to read the '}' to increment the file position
        }
        else if (c == '{')
        {
            // This is expected to be an unnamed block
            file.get();  // Important to read the '{' to increment the file position
            XFileObject *object = new XFileObject();
            if (!objectStack.empty())
            {
                XFileObject *parent = objectStack.top();
                if (parent)
                {
                    parent->children.push_back(object);
                    object->parent = parent;
                }
            }
            else
            {
                // throw exception here, this case is unexpected
            }
            objectStack.push(object);
        }
        else if (c == '/')
        {
            readComment();
        }
        else // Just read the next character and continue the loop.
        {
            file.get();
        }
    }
    //std::cout << "anim " << animTicksPerSecond << std::endl;

    file.close();
}

bool XFile::isAlpha(char c)
{
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z')
    {
        return true;
    }
    return false;
}

bool XFile::isNumeric(char c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}

std::string XFile::readString()
{
    std::string theString;
    while (true)
    {
        char c = file.get();
        theString += c;
        c = file.peek();
        if (file.eof())
        {
            //printf("read string %s\n", theString.c_str());
            return theString;
        }
        if (!isAlpha(c) && !isNumeric(c) && c != '_')
        {
            //printf("read string %s\n", theString.c_str());
            return theString;
        }
    }
}

float XFile::readNumber()
{
    std::string str;
    while (true)
    {
        char c = file.get();
        str += c;
        c = file.peek();
        if (file.eof())
            return (float) atof(str.c_str());
        if (!isNumeric(c) && c != '.'  && c != '-')
            return (float) atof(str.c_str());
    }
}

/**
 *  Read until end of line reached, ignoring content
 */
void XFile::readComment()
{
    while (true)
    {
        if (file.eof())
            return;

        char c = file.get();
        if (c == '\n')
        {
            return;
        }
    }
}

KeyframeAnimation *XFile::createKeyframeAnimation(int numberOfVertices)
{
    KeyframeAnimation *anim = new KeyframeAnimation(numberOfVertices);

    // Create the bones and set parent name for each bone.
    std::vector<XFileObject *> frameObjects = getXFileObjectsByType("Frame");
    int nFrames = frameObjects.size();
    int boneIndex = 0;
    for (int i = 0; i < nFrames; ++i)
    {
        XFileObject *currentFrame = frameObjects[i];
        if (currentFrame->name.substr(0, 4) == std::string("Bone"))
        {
            printf("found bone %s\n", currentFrame->name.c_str());
            Bone *bone = new Bone(boneIndex);
            bone->setName(currentFrame->name);
            MatrixArray frameTransform = getFrameTransformForObject(currentFrame);
            XFileObject *parent = currentFrame->parent;
            Bone *currentBone = bone;
            while (parent != 0)
            {
                // First,set the parent name for current bone
                if (currentBone)
                {
                    // check if parent Bone object exists before actually setting the name.
                    if (anim->getBoneByName(parent->name))
                    {
                        currentBone->setParentName(parent->name);
                    }
                    currentBone = anim->getBoneByName(parent->name);
                }
                if (objectHasFrameTransform(parent))
                {
                    MatrixArray parentTransform = getFrameTransformForObject(parent);
                    frameTransform = MatrixArray::product(frameTransform, parentTransform);
                }
                parent = parent->parent;
            }
            frameTransform.print();
            bone->setFrameTransform(frameTransform);
            anim->addBone(bone);
            boneIndex++;
        }
    }
    
    // Set the skin weights and bone offset transforms.
    SkinWeights *skinWeights = anim->getSkinWeights();
    std::vector<XFileObject *> skinWeightObjects = getXFileObjectsByType("SkinWeights");
    int size = skinWeightObjects.size();
    for (int i = 0; i < size; ++i)
    {
        XFileObject *currentObject = skinWeightObjects[i];
        std::vector<std::string> strings = currentObject->strings;
        int nStrings = strings.size();
        if (nStrings != 1)
        {
            // TODO: handle this error
            printf("Error - SkinWeights does not have a bone name as expected\n");
        }
        else
        {
            std::string boneName = currentObject->strings[0];
            printf("bone name = %s\n", boneName.c_str());
            int boneIndex = anim->getIndexForBone(boneName);
            printf("index = %d\n", boneIndex);
            std::vector<float> numbers = currentObject->numbers;
            int nNumbers = numbers.size();
            if (!nNumbers) 
            {
                // TODO: Handle this error
                printf("Error - no bone weights\n");
                continue;
            }
            int nWeights = (int) numbers[0];
            for (int k = 1; k <= nWeights; ++k)
            {
                int vertexIndex = (int) numbers[k];
                float weight = numbers[nWeights + k];
                skinWeights->addVertexBoneWeight(vertexIndex, (float) boneIndex, weight);
            }
            int offsetMatrixStartIndex = nWeights * 2 + 1;
            MatrixArray offsetMatrix;
            for (int m = 0; m < 16; ++m)
            {
                offsetMatrix.transform[m] = numbers[offsetMatrixStartIndex++];
            }
            Bone *theBone = anim->getBoneByName(boneName);
            if (theBone)
            {
                theBone->setOffsetTransform(offsetMatrix);
                printf("offset matrix bone %s\n", boneName.c_str());
                offsetMatrix.print();
            }
            else
            {
                // TODO: Handle this error properly - did not find offset matrix for bone boneName
                printf("ERROR -  did not find offset matrix for bone %s\n", boneName.c_str());
            }
        }
    }

    // Set the animation transforms
    std::vector<XFileObject *> animationObjects = getXFileObjectsByType("Animation");
    int nAnimations = animationObjects.size(); // Should be one for each bone
    for (int i = 0; i < nAnimations; ++i)
    {
        XFileObject *currentObject = animationObjects[i];
        // Should have a child element that represents the name of the bone.
        std::vector<XFileObject *> children = currentObject->children;
        int nChildren = children.size();
        if (nChildren == 2) // 1st child is name of the bone, 2nd child is the AnimationKey object.
        {
            std::string boneName = children[0]->strings[0];
            int boneIndex = anim->getIndexForBone(boneName);
            BoneAnimation *boneAnimation = new BoneAnimation(boneIndex);
            printf("boneIndex = %d\n", boneIndex);
            std::vector<float> numbers = children[1]->numbers;
            int nKeyframes = (int) numbers[1];
            if (i == 0)
            {  
                // All of the animation objects should have the same number of keyframes, so only set it once.
                anim->setNumberOfKeyframes(nKeyframes);
            }
            int numbersIndex = 2;
            for (int animIndex = 0; animIndex < nKeyframes; ++animIndex)
            {
                MatrixArray keyframeTransform;  // The next keyframe transformation in the AnimationKey object.
                numbersIndex++;  // number of keyframes
                int nValues = (int) numbers[numbersIndex++];  // Expected to be 16, as in 4x4 matrix
                for (int valueIndex = 0; valueIndex < nValues; ++valueIndex)
                {
                    keyframeTransform.setValue(valueIndex, numbers[numbersIndex++]);
                }
                boneAnimation->addBoneTransform(keyframeTransform);
            }
            anim->addBoneAnimation(boneAnimation);
        }
        else
        {
            // TODO: Report error - Animation object does not have the expected elements
        }
    }
    return anim;
}

std::vector<XFileObject *> XFile::getXFileObjectsByType(std::string type)
{
    XFileObjectTypeCollector collector(rootObjects);
    return collector.getMatchingObjects(type);
}


XFileObject *XFile::getRootFrame()
{
    int size = rootObjects.size();
    for (int i = 0; i < size; ++i)
    {
        XFileObject *current = rootObjects[i];
        if (current->name == std::string("RootFrame"))
        {
            return current;
        }
    }
    return 0;
}

/**
 *  Return 0 if @object has no FrameTransformMatrix
 */
MatrixArray XFile::getFrameTransformForObject(XFileObject *object)
{
    std::vector<XFileObject *> children = object->children;
    int size = children.size();
    for (int i = 0; i < size; ++i)
    {
        XFileObject *currentChild = children[i];
        if (currentChild->type == std::string("FrameTransformMatrix"))
        {
            std::vector<float> floatValues = currentChild->numbers;
            if (floatValues.size() != 16)
            {
                // Error: Expected 16 floats
            }
            MatrixArray matrix;
            for (int j = 0; j < 16; ++j)
            {
                matrix.setValue(j, floatValues[j]);
            }
            return matrix;
        }
    }
    MatrixArray m;
    return m;
}

bool XFile::objectHasFrameTransform(XFileObject *object)
{
    std::vector<XFileObject *> children = object->children;
    int size = children.size();
    for (int i = 0; i < size; ++i)
    {
        XFileObject *currentChild = children[i];
        if (currentChild->type == std::string("FrameTransformMatrix"))
        {
            return true;
        }
    }
    return false;
}
