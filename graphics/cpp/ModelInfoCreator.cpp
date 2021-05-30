#include "ModelInfoCreator.h"

#include "GraphicsSystem.h"
#include "Graphics.h"
#include "KeyFrameAnimation.h"
#include "util.h"

#include <iostream>

ModelInfoCreator::ModelInfoCreator(DaeColladaNode *theColladaNode)
: colladaNode(theColladaNode), xFile(0)
{
    modelInfo = new ModelInfo();
}

ModelInfoCreator::ModelInfoCreator(XFile *theXFile)
: xFile(theXFile), colladaNode(0)
{
    modelInfo = new ModelInfo();
}

ModelInfo *ModelInfoCreator::getModelInfo()
{
    if (colladaNode)
        return createFromColladaNode();
    if (xFile)
        return createFromXFile();
    return 0;
}

ModelInfo *ModelInfoCreator::createFromColladaNode()
{

    modelInfo->setFileName(colladaNode->getFileName());

    // Get the mesh from the Collada node
    DaeLibraryGeometriesNode *libGeom = colladaNode->getLibraryGeometries();
    std::vector<DaeGeometryNode *> geometries = libGeom->getGeometries();
    if (geometries.size() != 1)
    {
        return 0;
    }
    DaeGeometryNode *geomNode = geometries[0];
    std::vector<DaeMeshNode *> meshes = geomNode->getMeshes();
    if (meshes.size() != 1)
    {
        return 0;
    }
    DaeMeshNode *meshNode = meshes[0];
    
    // From the mesh node get the vertex positions and normals
    std::vector<DaeSourceNode *> sources = meshNode->getSources();
    DaeSourceNode *positionSource = 0;
    DaeSourceNode *normalSource = 0;
    std::vector<DaeSourceNode *>::iterator iter = sources.begin();
    std::vector<DaeSourceNode *>::iterator end = sources.end();
    for (; iter != end; iter++)
    {
        DaeSourceNode *currentSource = *iter;
        std::string sourceId = currentSource->getId();
        if (sourceId.find("Position") != std::string::npos)
        {
            positionSource = currentSource;
        }
        else if (sourceId.find("Normals") != std::string::npos)
        {
            normalSource = currentSource;
        }
    }
    if (!positionSource || !normalSource)
    {
        return 0;
    }
    
    // Set the vertex values in the model
    DaeFloatArrayNode *floatArrayNode = positionSource->getFloatArrayNode();
    float *vertices = floatArrayNode->getValues();
    int sizeOfVertices = floatArrayNode->getCount();
    //printf("sizeOfVertices = %d\n", sizeOfVertices);
    int vertexCount = sizeOfVertices / 3;
    //printf("vertex count = %d\n", vertexCount);
    modelInfo->vertexPositions = vertices;
    modelInfo->vertexCount = vertexCount; 
    // From the mesh node get the triangles = index buffer into position and normals arrays
    std::vector<DaeTrianglesNode *> triangles = meshNode->getTriangles();
    // Expect one triangles node
    if (triangles.size() != 1)
    {
        return 0;
    }
    
    // The indices for the vertices and normals is interleaved
    // We need to separate these indices into non-interleaved arrays
    // TODO: deallocate the interleaved array after copying the data
    DaeTrianglesNode *trianglesNode = triangles[0];
    int numberOfValues = trianglesNode->getNumberOfValues();
    int *allIndices = trianglesNode->getValues();
    int triangleCount = numberOfValues / 6;
 
    //printf("triangle count = %d\n", triangleCount);
    int indexCount = triangleCount * 3;
    unsigned short *indices = new unsigned short[indexCount]; 
    int j = 0;
    for (int i = 0; i < numberOfValues; i = i + 2)
    {
        indices[j] = allIndices[i];
        // Swap clockwise ordering of the vertices for the triangles.
        if (j % 3 == 2)
        {
            int temp = indices[j];
            indices[j] = indices[j - 1];
            indices[j - 1] = temp;
        }
        j++;
    } 
    modelInfo->indices = indices;
    modelInfo->indexCount = indexCount;

    // Create some color values - this is temporary for testing purposes
    modelInfo->hasColorValues = true;
    int colorValueCount = vertexCount * 3;
    float *colors = new float[colorValueCount];
    for (int i = 0; i < colorValueCount; i++)
    {
        colors[i] = 1.0f / ((float) (rand() % 10) + 1.0f);
    }
    modelInfo->colorValueCount = colorValueCount;
    modelInfo->colorValues = colors;
    return modelInfo;
}

ModelInfo *ModelInfoCreator::createFromXFile()
{
    // Look for the mesh in a sub-frame of the root frame
    // Currently assumes there is only a single mesh
    modelInfo->setFileName(xFile->fileName);
    std::vector<XFileObject *> xObjects = xFile->getRootObjects();
   
    XFileObject *meshObject = 0;
    XFileObject *animationSet = 0;
    std::vector<XFileObject *> animationSets = xFile->getXFileObjectsByType("AnimationSet");
    printf("animationSets.size() = %d\n", animationSets.size());
    if (animationSets.size() > 0)
    {
        printf("Found animation set\n");
        animationSet = animationSets[0];
    }
    std::vector<XFileObject *> meshes = xFile->getXFileObjectsByType("Mesh");
    printf("meshes.size() = %d\n", meshes.size());
    if (meshes.size() > 0)
    {
        printf("Found mesh\n");
        meshObject = meshes[0];
    }
    int numberOfVertices = 0;
    if (meshObject)
    {
        // TODO: For some reason the x file exported by Blender has many
        // duplicate vertices.  These duplicates should be removed.  This
        // is not easy since the corresponding set of index values must be 
        // modified.
        std::vector<float> numbers = meshObject->numbers;
        numberOfVertices = (int) numbers[0];
        float *vertexPositions = new float[numberOfVertices * 3];
        int i = 1;
        for (; i <= numberOfVertices * 3; i = i + 3)
        {
            vertexPositions[i - 1] = numbers[i];
            vertexPositions[i] = numbers[i + 1];
            vertexPositions[i + 1] = numbers[i + 2];
        }
        int numberOfPrimitives = (int) numbers[i++];
        int numberOfIndices = 0;  // This value will accumulate
        std::vector<int> indices;
        for (int p = 0; p < numberOfPrimitives; ++p)
        {
            // Make note of the winding order here
            // It is counter-clockwise
            int verticesPerPrimitive = (int) numbers[i++];
            if (verticesPerPrimitive == 4) // quads
            {
                // Create 2 triangles from the quad
                int index1 = (int) numbers[i++];
                int index2 = (int) numbers[i++];
                int index3 = (int) numbers[i++];
                int index4 = (int) numbers[i++];
                Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
                if (graphics->usingDirect3D())
                {
                    indices.push_back(index1);
                    indices.push_back(index3);
                    indices.push_back(index2);
                    indices.push_back(index1);
                    indices.push_back(index4);
                    indices.push_back(index3);
                }
                else
                {
                    indices.push_back(index1);
                    indices.push_back(index2);
                    indices.push_back(index3);
                    indices.push_back(index1);
                    indices.push_back(index3);
                    indices.push_back(index4);
                }
                numberOfIndices += 6;
            }
            else if (verticesPerPrimitive == 3) // triangles
            {
                int index1 = (int) numbers[i++];
                int index3 = (int) numbers[i++];
                int index2 = (int) numbers[i++];
                Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
                if (graphics->usingDirect3D())
                {
                    indices.push_back(index1);
                    indices.push_back(index2);
                    indices.push_back(index3);
                }
                else
                {
                    indices.push_back(index1);
                    indices.push_back(index3);
                    indices.push_back(index2);
                }
            }
            else
            {
                return 0;  // TODO: Error - unexpected primitive
            }
        }
        unsigned short *indicesArray = new unsigned short[indices.size()];
        int nIndices = (int) indices.size();
        for (int k = 0; k < nIndices; ++k)
        {
            indicesArray[k] = indices[k];
        }
        modelInfo->vertexPositions = vertexPositions;
        modelInfo->vertexCount = numberOfVertices;  
        modelInfo->indices = indicesArray;
        modelInfo->indexCount = indices.size();


        // Create some color values - this is temporary for testing purposes
        modelInfo->hasColorValues = true;
        int colorValueCount = numberOfVertices * 3;
        float *colors = new float[colorValueCount];
        for (int i = 0; i < colorValueCount; i++)
        {
            colors[i] = 1.0f / ((float) (rand() % 10) + 1.0f);
        }
        modelInfo->colorValueCount = colorValueCount;
        modelInfo->colorValues = colors;

        // Now look for the bone skin weights
        std::vector<XFileObject *> boneSkinWeightObjects;
        std::vector<XFileObject *> meshChildren = meshObject->children;
        int nMeshChildren = (int) meshChildren.size();
        for (int j = 0; j < nMeshChildren; ++j)
        { 
            XFileObject *child = meshChildren[j];
            if (child->type == std::string("SkinWeights"))
            {
                // Found SkinWeight object
                printf("Found SkinWeight object\n");
                boneSkinWeightObjects.push_back(child);
            }
        }
        int numberOfBones = boneSkinWeightObjects.size();
        printf("# of bones = %d\n", numberOfBones);

        // Get the texture coordinates
        std::vector<XFileObject *> texCoordObjects = xFile->getXFileObjectsByType("MeshTextureCoords");
        if (texCoordObjects.size() > 0)
        {
            if (texCoordObjects.size() > 1)
            {
                fatalError("Only expected 1 set of texture coords.");
            }
            XFileObject *texCoordsObject = texCoordObjects[0];
            std::vector<float> texCoordValues = texCoordsObject->numbers;
            int count = (int) texCoordValues[0];
            float *texCoords = new float[count * 2];
            for (int i = 0; i < count * 2; ++i)
            {
                texCoords[i] = texCoordValues[i + 1];
                if (texCoords[i] < 0)
                    texCoords[i] = -texCoords[i];
            }
            modelInfo->textureCoords = texCoords;
            modelInfo->hasTexture = true;
            modelInfo->hasColorValues = false;
        }

        // Get the normals
        std::vector<XFileObject *> normalObjects = xFile->getXFileObjectsByType("MeshNormals");
        if (normalObjects.size() > 0)
        {
            if (normalObjects.size() > 1)
            {
                fatalError("Only expected 1 set of texture coords.");
            }
            XFileObject *normalObject = normalObjects[0];
            std::vector<float> numbers = normalObject->numbers;
            int count = (int) numbers[0];
            int nNormalValues = count * 3;
            float *normals = new float[nNormalValues];
            for (int i = 0; i < nNormalValues; ++i)
            {
                normals[i] = numbers[i + 1];
            }
            modelInfo->normals = normals;
            modelInfo->hasNormals = true;
        }

    }
    KeyframeAnimation *animation = xFile->createKeyframeAnimation(numberOfVertices);
    if (animation->getNumberOfKeyframes() == 0)
    {
        animation = 0;
    }
    modelInfo->animation = animation;
    return modelInfo;
}
