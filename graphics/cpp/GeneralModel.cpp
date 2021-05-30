#include "GeneralModel.h"

#include "Camera.h"
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

GeneralModel::GeneralModel()
: Model(), wasInitialized(0)
{
}

Model *GeneralModel::create(std::string modelName)
{
    //XFile *xFile = new XFile("models/box_bend_3.x");
    //XFile *xFile = new XFile("models/box_animation_with_tex_coords.x");
    //XFile *xFile = new XFile("models/boxes_2_bones_tex_coords.x");
    //XFile *xFile = new XFile("models/boxes_3_bones.x");
    //XFile *xFile = new XFile("models/boxes_3_bones_tex_coords.x");
    //XFile *xFile = new XFile("models/arms_moving_down.x");
    //XFile *xFile = new XFile("models/confused_worm.x");
    printf("modelName = %s\n", modelName.c_str());
    XFile *xFile = new XFile(std::string("models/") + modelName + ".x");
    ModelInfoCreator infoCreator(xFile);
    ModelInfo *modelInfo = infoCreator.getModelInfo();
    if (!modelInfo)
    {
        fatalError("Failed to create ModelInfo object: general_model");
    }
    std::string fileName = "general_model:" + modelName;
    modelInfo->setFileName(fileName);

    if (modelInfo->hasTexture)
    {
        //modelInfo->textureFile = std::string("images/green.bmp");
    }

    if (modelInfo->animation)
    {
        modelInfo->hasSecondTexture = true;  // TODO: Should be false if no animation

        // Set up bone indices and bone weights in texture coordinates
        SkinWeights *skinWeightsObject = modelInfo->animation->getSkinWeights();
        int nVertices = skinWeightsObject->getNumberOfVertices();
        float *texCoords2 = new float[nVertices * 2];
        float *texCoords3 = new float[nVertices * 2];
        modelInfo->hasThirdTexture = true;
        int vertexCount = modelInfo->vertexCount;
        for (int vertexIndex = 0; vertexIndex < vertexCount; ++vertexIndex)
        {
            VertexBoneWeights boneWeights = skinWeightsObject->getBoneWeights(vertexIndex);
            float *indices = boneWeights.getBoneIndices();
            float *weights = boneWeights.getBoneWeights();
            texCoords2[vertexIndex * 2] = indices[0];
            texCoords2[vertexIndex * 2 + 1] = indices[1];
            texCoords3[vertexIndex * 2] = weights[0];
            texCoords3[vertexIndex * 2 + 1] = weights[1];
        }
        modelInfo->textureCoords2 = texCoords2;
        modelInfo->textureFile2 = std::string("[none]");
        modelInfo->textureCoords3 = texCoords3;
        modelInfo->textureFile3 = std::string("[none]");
        // End setting bone weights and indices
    }


    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    ShaderInfo *vertexShaderInfo = 0;
    std::string vertexShaderFile;
    if (model->hasAnimation())
    {
        vertexShaderFile = std::string("shaders/basic_skinning_with_texture.cgvert");
    }
    else if (modelInfo->hasTexture)
    {
        vertexShaderFile = std::string("shaders/basic_texture_and_light.cgvert");

        //vertexShaderFile = std::string("shaders/basic_texture_and_phong.cgvert");
    }
    else
    {
        vertexShaderFile = std::string("shaders/vertex_shader.cgvert");
    }
    vertexShaderInfo = new ShaderInfo(ShaderInfo::VERTEX_SHADER, 
        ShaderInfo::TEXT_FILE, vertexShaderFile, model->getVertexType(), fileName);

    ShaderReference *vertexShader = graphics->createShader(vertexShaderInfo);

    if (vertexShader && model->hasAnimation())
    {
        vertexShader->bindParameterName("offsetTransforms", ShaderInfo::FLOAT_ARRAY);
        vertexShader->bindParameterName("boneTransforms", ShaderInfo::FLOAT_ARRAY);
        //vertexShader->bindParameterName("skinWeights", ShaderInfo::FLOAT_ARRAY);

        // Not used now, but maybe needed in the future
        //vertexShader->bindParameterName("frameTransforms", ShaderInfo::FLOAT_ARRAY);
	} 
	else if (vertexShader && modelInfo->hasTexture) 
	{
            vertexShader->bindParameterName("eyePositionArray", ShaderInfo::FLOAT_ARRAY);
			vertexShader->bindParameterName("worldMatrix", ShaderInfo::MATRIX);
	}
    
    vertexShader->bindParameterName("worldViewProj", ShaderInfo::MATRIX);

    std::string fragShaderFile;
    if (model->hasAnimation())  
    {
        // TODO: Support texture + lighting in the shader for animated models.
        fragShaderFile = std::string("shaders/basic_texture.cgfrag");
    }
    else if (modelInfo->hasTexture)
    {
		// very basic fragment shader
        fragShaderFile = std::string("shaders/basic_texture_and_light.cgfrag");

		// fragment shader using phong
        //fragShaderFile = std::string("shaders/basic_texture_with_phong.cgfrag");
    }
    else
    {
        fragShaderFile = std::string("shaders/blue_pixel_shader.cgfrag");
    }
    ShaderInfo *fragmentShaderInfo = new ShaderInfo(ShaderInfo::FRAGMENT_SHADER, 
        ShaderInfo::TEXT_FILE, fragShaderFile, model->getVertexType(), fileName);
    ShaderReference *fragmentShader = graphics->createShader(fragmentShaderInfo);

    model->setVertexShader(vertexShader);
    model->setFragmentShader(fragmentShader);
    model->useLighting = true;
    model->useCulling = true;

    if (fragmentShader && modelInfo->hasTexture)
    {
        //if (graphics->usingDirect3D())
        //{
            fragmentShader->bindParameterName("testTexture", ShaderInfo::SAMPLER_2D);
            //fragmentShader->bindParameterName("eyePosition", ShaderInfo::FLOAT_ARRAY);
        //}
    }

    delete vertexShaderInfo;
    delete fragmentShaderInfo;


    return model;
}

void GeneralModel::initialize()
{
    if (fragmentShader)
    {
        GraphicsSystem *gs = GraphicsSystem::getInstance();
        Graphics *graphics = gs->getGraphics();
        if (texture && graphics->usingDirect3D())
        {
            //fragmentShader->setUniformTexture("testTexture", texture);
        }
		if (texture) 
		{
			//float eyePosition[3];
			//eyePosition[0] = Camera::getInstance()->getEyeX();
			//eyePosition[1] = Camera::getInstance()->getEyeY();
			//eyePosition[2] = Camera::getInstance()->getEyeZ();
			//fragmentShader->setUniformArray("eyePosition", 0, 3, eyePosition);
		}
    }

    if (!animation)
        return;

    totalTime = 5.0f;
    currentTime = 0.0f;


    numberOfKeyframes = animation->getNumberOfKeyframes();
    printf("numberOfKeyframes = %f\n", numberOfKeyframes);
    numberOfBones = animation->getNumberOfBones();
    printf("numberOfBones = %f\n", numberOfBones);

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

    // Allocate memory for boneTransforms
    int btSize = numberOfBones * 16;
    boneTransforms = new float[btSize];

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
    
    // The shader receives the data in groups of 4 floats, so the size is numberOfBones * 4
    vertexShader->setUniformArray("offsetTransforms", 0, (int) numberOfBones * 4, offsetTransforms);


}

void GeneralModel::update(float deltaSeconds)
{
    if (!wasInitialized)
    {
        initialize();
        wasInitialized = true;
    }
}

void GeneralModel::setCurrentAnimationTime(float time)
{
    currentTime = time;
}

void GeneralModel::setTotalAnimationTime(float time)
{
    totalTime = time;
}

void GeneralModel::draw(Graphics *graphics, bool picking)
{
    if (texture && !graphics->usingDirect3D() && !picking)
    {
        texture->activate();
    }
    if (animation)
    {
        float elapsedTimeRatio = currentTime / totalTime;
        int currentKeyframe = (int) ((float) (elapsedTimeRatio * ((float) numberOfKeyframes - 1.0f)));
       
        float perFrameTime = totalTime / (numberOfKeyframes - 1.0f);
        float frameStartTime = (float) currentKeyframe * perFrameTime;
        float frameTimeRatio = (currentTime - frameStartTime) / perFrameTime;

        int boneTransformsIndex = 0;
        for (int boneIndex = 0; boneIndex < numberOfBones; ++boneIndex)
        {
            int currentFrameStartIndex = (boneIndex * numberOfKeyframes + currentKeyframe) * 16;
            int nextFrameStartIndex = currentFrameStartIndex + 16;

            for (int matrixIndex = 0; matrixIndex < 16; ++matrixIndex)
            {
                boneTransforms[boneTransformsIndex++] = keyframeTransforms[currentFrameStartIndex + matrixIndex]
                * (1.0f - frameTimeRatio) + keyframeTransforms[nextFrameStartIndex + matrixIndex] * frameTimeRatio;
            }
        }
        vertexShader->setUniformArray("boneTransforms", 0, (int) numberOfBones * 4, boneTransforms);
    } else if (vertexShader && texture)
	{
			float eyePosition[3];
			eyePosition[0] = Camera::getInstance()->getEyeX();
			eyePosition[1] = Camera::getInstance()->getEyeY();
			eyePosition[2] = Camera::getInstance()->getEyeZ();
			vertexShader->setUniformArray("eyePositionArray", 0, 3, eyePosition);

            if (graphics->usingDirect3D())
            {
                vertexShader->setUniformParameter("worldMatrix", worldMatrix);
            }
            else
            {
                vertexShader->setUniformMatrix("worldMatrix", worldMatrix);
            }
	}

    glDisable(GL_TEXTURE_2D);  // TODO: Use Graphics object

    if (vertexShader && !picking)
    {
        graphics->setShaderIsActive(true);
        float *worldViewProj = graphics->getModelViewProjectionMatrix();
        if (graphics->usingDirect3D())
        {
            vertexShader->setUniformParameter("worldViewProj", worldViewProj);
        }
        else
        {
            vertexShader->setUniformMatrix("worldViewProj", worldViewProj);
        }
        vertexShader->activate();
    }
    if (fragmentShader && !picking)
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
        graphics->setShaderIsActive(false);
        vertexShader->deactivate();
    }
    if (fragmentShader)
    {
        graphics->setShaderIsActive(false);
        fragmentShader->deactivate();
    }
    if (texture)
    {
        texture->deactivate();
    }
}