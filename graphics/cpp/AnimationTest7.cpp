#include "AnimationTest7.h"
#include "ModelInfo.h"
#include "ModelCreator.h"
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "LinearMath/btTransform.h"
#include <math.h>
#include "XFile.h"
#include "ModelInfoCreator.h"
#include "util.h"
#include "ShaderInfo.h"
#include "ShaderReference.h"

  // TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"

AnimationTest7::AnimationTest7()
: Model(), wasInitialized(0)
{
}

Model *AnimationTest7::create()
{
    //XFile *xFile = new XFile("models/box_bend_3.x");
    //XFile *xFile = new XFile("models/box_animation_with_tex_coords.x");
    //XFile *xFile = new XFile("models/boxes_2_bones_tex_coords.x");
    //XFile *xFile = new XFile("models/boxes_3_bones.x");
    XFile *xFile = new XFile("models/boxes_3_bones_tex_coords.x");
    ModelInfoCreator infoCreator(xFile);
    ModelInfo *modelInfo = infoCreator.getModelInfo();
    if (!modelInfo)
    {
        fatalError("Failed to create ModelInfo object: general_model");
    }
    modelInfo->setFileName("animation_test_7");

    if (modelInfo->hasTexture)
    {
        modelInfo->textureFile = std::string("images/Granite.bmp");
    }
    modelInfo->hasSecondTexture = true;  // TODO: Should be false if no animation
    int vertexCount = modelInfo->vertexCount;
    int texCoordCount = vertexCount * 2;

    // Pass each vertex index using texture coordinates so
    // that the shader knows which vertex it is operating on.
    float *texCoords2 = new float[texCoordCount];
    int vertexIndex = 0;
    for (int i = 0; i < texCoordCount; i += 2)
    {
        texCoords2[i] = (float) vertexIndex;
        vertexIndex++;
        texCoords2[i + 1] = 0;
    }
    modelInfo->textureCoords2 = texCoords2;
    modelInfo->textureFile2 = std::string("[none]");

    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    ShaderInfo *vertexShaderInfo = 0;
    std::string vertexShaderFile;
    if (model->hasAnimation())
    {
        vertexShaderFile = std::string("shaders/basic_anim_with_texture_uniform_weights.cgvert");
    }
    else if (modelInfo->hasTexture)
    {
        vertexShaderFile = std::string("shaders/basic_texture.cgvert");
    }
    else
    {
        vertexShaderFile = std::string("shaders/vertex_shader.cgvert");
    }
    vertexShaderInfo = new ShaderInfo(ShaderInfo::VERTEX_SHADER, 
        ShaderInfo::TEXT_FILE, vertexShaderFile, model->getVertexType(), "anim_test_7_context");

    ShaderReference *vertexShader = graphics->createShader(vertexShaderInfo);

    if (vertexShader && model->hasAnimation())
    {
        vertexShader->bindParameterName("numberOfKeyframes", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("numberOfBones", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("totalTime", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("currentTime", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("offsetTransforms", ShaderInfo::FLOAT_ARRAY);
        vertexShader->bindParameterName("keyframeTransforms", ShaderInfo::FLOAT_ARRAY);
        vertexShader->bindParameterName("skinWeights", ShaderInfo::FLOAT_ARRAY);

        // Not used now, but maybe needed in the future
        //vertexShader->bindParameterName("frameTransforms", ShaderInfo::FLOAT_ARRAY);
    }
    
    vertexShader->bindParameterName("worldViewProj", ShaderInfo::MATRIX);

    std::string fragShaderFile;
    if (modelInfo->hasTexture)
    {
        fragShaderFile = std::string("shaders/basic_texture.cgfrag");
    }
    else
    {
        fragShaderFile = std::string("shaders/blue_pixel_shader.cgfrag");
    }
    ShaderInfo *fragmentShaderInfo = new ShaderInfo(ShaderInfo::FRAGMENT_SHADER, 
        ShaderInfo::TEXT_FILE, fragShaderFile, model->getVertexType(), "anim_test_7_context");
    ShaderReference *fragmentShader = graphics->createShader(fragmentShaderInfo);

    model->setVertexShader(vertexShader);
    model->setFragmentShader(fragmentShader);
    model->useLighting = true;
    model->useCulling = true;

    if (fragmentShader && modelInfo->hasTexture)
    {
        if (graphics->usingDirect3D())
        {
            fragmentShader->bindParameterName("testTexture", ShaderInfo::SAMPLER_2D);
        }
    }

    delete vertexShaderInfo;
    delete fragmentShaderInfo;


    return model;
}

void AnimationTest7::initialize()
{
    if (fragmentShader)
    {
        GraphicsSystem *gs = GraphicsSystem::getInstance();
        Graphics *graphics = gs->getGraphics();
        if (texture && graphics->usingDirect3D())
        {
            fragmentShader->setUniformTexture("testTexture", texture);
        }
    }

    if (!animation)
        return;

    totalTime = 2.0f;
    vertexShader->setUniformParameter("totalTime", &totalTime);
    currentTime = 0.0f;

    if (!animation)
    {
        // TODO: Handle this error
        printf("No animation object initialized.\n");
    }

    numberOfKeyframes = animation->getNumberOfKeyframes();
    printf("numberOfKeyframes = %d\n", numberOfKeyframes);
    numberOfBones = animation->getNumberOfBones();
    printf("numberOfBones = %d\n", numberOfBones);

    // Set up the offset matrices block
    offsetTransforms = new float[(int) numberOfBones * 16];
    int offsetBlockIndex = 0; // The index into offsetTransforms
    for (int boneIndex = 0; boneIndex < numberOfBones; ++boneIndex)
    {
        Bone *currentBone = animation->getBoneByIndex(boneIndex);
        if (!currentBone)
        {
            // TODO: Handle this better
            printf("ERROR - bone not found for index %d\n", boneIndex);
            continue;
        }
        MatrixArray transform = currentBone->getOffsetTransform();
        printf("offset matrices\n");
        transform.print();
        for (int t = 0; t < 16; ++t)
        {
            offsetTransforms[offsetBlockIndex++] = transform.transform[t];
        }
    }

    // Set up the frame matrices block
    // Not used now, but maybe needed in the future
    /*
    frameTransforms = new float[(int) numberOfBones * 16];
    int frameBlockIndex = 0; // The index into offsetTransforms
    for (int boneIndex = 0; boneIndex < numberOfBones; ++boneIndex)
    {
        Bone *currentBone = animation->getBoneByIndex(boneIndex);
        if (!currentBone)
        {
            // TODO: Handle this better
            printf("ERROR - bone not found for index %d\n", boneIndex);
            continue;
        }
        MatrixArray transform = currentBone->getFrameTransform();
        printf("frame matrices\n");
        transform.print();
        for (int t = 0; t < 16; ++t)
        {
            frameTransforms[frameBlockIndex++] = transform.transform[t];
        }
    }
    */

    // Set up skin weights block
    SkinWeights *skinWeightsObject = animation->getSkinWeights();
    int nVertices = skinWeightsObject->getNumberOfVertices();
    printf("# of vertices = %d\n", nVertices);
    skinWeights = new float[nVertices * 8];
    int skinWeightsIndex = 0;
    for (int vertexIndex = 0; vertexIndex < nVertices; ++vertexIndex)
    {
        VertexBoneWeights boneWeights = skinWeightsObject->getBoneWeights(vertexIndex);
        float *indices = boneWeights.getBoneIndices();
        float *weights = boneWeights.getBoneWeights();
        skinWeights[skinWeightsIndex++] = indices[0];
        skinWeights[skinWeightsIndex++] = indices[1];
        skinWeights[skinWeightsIndex++] = indices[2];
        skinWeights[skinWeightsIndex++] = indices[3];
        skinWeights[skinWeightsIndex++] = weights[0];
        skinWeights[skinWeightsIndex++] = weights[1];
        skinWeights[skinWeightsIndex++] = weights[2];
        skinWeights[skinWeightsIndex++] = weights[3];
    }
    for (int z = 0; z < nVertices * 8; ++z)
    {
        printf("%f    ", skinWeights[z]);
        if ((z + 1) % 4 == 0) 
        {
            printf("\n");
        }
    }

    // Set up the keyframe animation transforms block
    int kfSize = numberOfBones * numberOfKeyframes * 16;
    keyframeTransforms = new float[kfSize];
    int keyframeBlockIndex = 0;
    for (int boneIndex = 0; boneIndex < numberOfBones; ++boneIndex)
    {
        BoneAnimation *boneAnim = animation->getBoneAnimationByBoneIndex(boneIndex);
        if (!boneAnim)
        {
            // TODO: Handle this error properly
            printf("Error: bone animation not present for bone index %d\n", boneIndex);
            continue;
        }
        int nBoneTransforms = boneAnim->getNumberOfTransforms();
        if (nBoneTransforms != numberOfKeyframes)
        {
            printf("Error - expected %d keyframes, found %d\n", numberOfKeyframes, nBoneTransforms);
            continue;
        }
        for (int keyframe = 0; keyframe < numberOfKeyframes; ++keyframe)
        {
            MatrixArray transform = boneAnim->getTransform(keyframe);
            BoneAnimation *parent = animation->getParentBoneAnimation(boneAnim);
            while (parent)
            {
                printf("parent index = %d\n", parent->getBoneIndex());
                MatrixArray parentTransform = parent->getTransform(keyframe);
                transform = MatrixArray::product(transform, parentTransform);
                parent = animation->getParentBoneAnimation(parent);
            }
            
            printf("animation matrices\n");
            transform.print();
            for (int f = 0; f < 16; ++f)
            {
                keyframeTransforms[keyframeBlockIndex++] = transform.transform[f];
            }
        }
    }

    float f_numberOfKeyframes = (float) numberOfKeyframes;
    vertexShader->setUniformParameter("numberOfKeyframes", &f_numberOfKeyframes);
    float f_numberOfBones = (float) numberOfBones;
    vertexShader->setUniformParameter("numberOfBones", &f_numberOfBones);
    
    // The shader receives the data in groups of 4 floats, so the size is numberOfBones * 4
    vertexShader->setUniformArray("offsetTransforms", 0, (int) numberOfBones * 4, offsetTransforms);

    vertexShader->setUniformArray("skinWeights", 0, (int) nVertices * 2, skinWeights);
    vertexShader->setUniformArray("keyframeTransforms", 0, (int) numberOfBones * numberOfKeyframes * 4, keyframeTransforms);

    // Not used now, but maybe needed in the future
    //vertexShader->setUniformArray("frameTransforms", 0, (int) numberOfBones * 4, frameTransforms);
}

void AnimationTest7::update(float deltaSeconds)
{
    if (!wasInitialized)
    {
        initialize();
        wasInitialized = true;
    }
}

void AnimationTest7::setCurrentAnimationTime(float time)
{
    vertexShader->setUniformParameter("currentTime", &time);
}

void AnimationTest7::setTotalAnimationTime(float time)
{
    vertexShader->setUniformParameter("totalTime", &time);
}

void AnimationTest7::draw(Graphics *graphics, bool picking)
{
    glDisable(GL_TEXTURE_2D);  // TODO: Use Graphics object
    if (picking)
        currentTime = 0.0f;

    if (vertexShader)
    {
        graphics->setShaderIsActive(true);
        float *worldViewProj = graphics->getModelViewProjectionMatrix();
        vertexShader->setUniformParameter("worldViewProj", worldViewProj);
        vertexShader->activate();
    }
    if (fragmentShader)
    {
        graphics->setShaderIsActive(true);
        fragmentShader->activate();
    }
    if (!picking)
    {
        graphics->enableMode(Graphics::DEPTH_TEST);
        //graphics->enableMode(Graphics::TEXTURE_2D);
        if (useLighting)
            graphics->enableMode(Graphics::LIGHTING);
        else
            graphics->disableMode(Graphics::LIGHTING);
        if (useCulling)
            graphics->enableMode(Graphics::CULLING);
        else
            graphics->disableMode(Graphics::CULLING);
    }

    mesh->draw(graphics, picking);

    if (vertexShader)
    {
        vertexShader->deactivate();
    }
    if (fragmentShader)
    {
        fragmentShader->deactivate();
    }
    graphics->setShaderIsActive(false);
}