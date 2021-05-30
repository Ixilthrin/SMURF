#include "AnimationTest5.h"
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

AnimationTest5::AnimationTest5()
: Model(), wasInitialized(0)
{
}

Model *AnimationTest5::create()
{
    XFile *xFile = new XFile("models/box_bend.x");
    ModelInfoCreator infoCreator(xFile);
    ModelInfo *modelInfo = infoCreator.getModelInfo();
    if (!modelInfo)
    {
        fatalError("Failed to create ModelInfo object: animation_test_5");
    }
    modelInfo->setFileName("animation_test_5");
    modelInfo->hasSecondTexture = true;
    int vertexCount = modelInfo->vertexCount;
    int texCoordCount = vertexCount * 2;
    float *texCoords2 = new float[texCoordCount];
    for (int i = 0; i < texCoordCount; i += 2)
    {
        texCoords2[i] = (float) i;
        texCoords2[i + 1] = 0;
    }
    modelInfo->textureCoords2 = texCoords2;
    modelInfo->textureFile2 = std::string("[none]");

    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    ShaderInfo *vertexShaderInfo = new ShaderInfo(ShaderInfo::VERTEX_SHADER, 
        ShaderInfo::TEXT_FILE, std::string("shaders/anim_test_5.cgvert"), 
        model->getVertexType(), "anim_test_5_context");
    ShaderInfo *fragmentShaderInfo = new ShaderInfo(ShaderInfo::FRAGMENT_SHADER, 
        ShaderInfo::TEXT_FILE, std::string("shaders/anim_test_5.cgfrag"), 
        model->getVertexType(), "anim_test_5_context");

    ShaderReference *vertexShader = graphics->createShader(vertexShaderInfo);

    if (vertexShader)
    {
        vertexShader->bindParameterName("numberOfTransforms", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("totalTime", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("currentTime", ShaderInfo::FLOAT_VALUE);
        vertexShader->bindParameterName("frameTransforms", ShaderInfo::FLOAT_ARRAY);
        vertexShader->bindParameterName("worldViewProj", ShaderInfo::MATRIX);
    }

    ShaderReference *fragmentShader = graphics->createShader(fragmentShaderInfo);



    model->setVertexShader(vertexShader);
    model->setFragmentShader(fragmentShader);
    model->useLighting = false;
    model->useCulling = false;

    delete vertexShaderInfo;
    delete fragmentShaderInfo;


    return model;
}

void AnimationTest5::initialize()
{
    totalTime = 2.0f;
    vertexShader->setUniformParameter("totalTime", &totalTime);
    currentTime = 0.0f;

    numberOfTransforms = 11.0f;
    vertexShader->setUniformParameter("numberOfTransforms", &numberOfTransforms);
    float minAngle = 0.0f;
    float maxAngle = SIMD_HALF_PI;
    float deltaAngle = (maxAngle - minAngle) / (float) numberOfTransforms;
    btVector3 axisOfRotation(0.0f, 0.0f, 1.0f);   // z-axis
    transforms = new float[(int) numberOfTransforms * 16];
    for (int i = 0; i < numberOfTransforms; ++i)
    {
        // Rotation about z-axis
        btQuaternion rotation;
        float currentAngle = deltaAngle * (float) i;
        rotation.setRotation(axisOfRotation, currentAngle);
        btTransform transform;
        transform.setIdentity();
        transform.setRotation(rotation);
        int tIndex = i * 16;
        transform.getOpenGLMatrix(transforms + tIndex);
        transforms[tIndex + 15] = 1.0f;
    }
    // The shader receives the data in groups of 4 floats, so the size is numberOfTransforms * 4,

    vertexShader->setUniformArray("frameTransforms", 0, (int) numberOfTransforms * 4, transforms);

}

void AnimationTest5::update(float deltaSeconds)
{
    if (!wasInitialized)
    {
        initialize();
        wasInitialized = true;
    }
}

void AnimationTest5::setCurrentAnimationTime(float time)
{
    vertexShader->setUniformParameter("currentTime", &time);
}

void AnimationTest5::setTotalAnimationTime(float time)
{
    vertexShader->setUniformParameter("totalTime", &time);
}

void AnimationTest5::draw(Graphics *graphics, bool picking)
{
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