#include "AnimationTest3.h"
#include "ModelInfo.h"
#include "ModelCreator.h"
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "LinearMath/btTransform.h"
#include <math.h>

AnimationTest3::AnimationTest3()
: Model(), wasInitialized(0)
{
}

Model *AnimationTest3::create()
{
    float vertexPositions[] =
    {
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f,-1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
    };

    float texCoords[] =
    { 0.0f,0.0f,  1.0f,0.0f, 1.0f,1.0f, 0.0f,1.0f };

    unsigned short indices[] = { 0, 1, 2, 3, 0, 2 };

    ModelInfo *modelInfo = new ModelInfo();
    modelInfo->setFileName("animation_test_3");
    modelInfo->hasColorValues = false;
    modelInfo->hasTexture = true;
    modelInfo->textureCoords = texCoords;
    modelInfo->indexCount = 6;
    modelInfo->indices = indices;
    modelInfo->vertexCount = 4;
    modelInfo->vertexPositions = vertexPositions;
    modelInfo->textureFile = std::string("images/scenic.bmp");

    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    model->setUseLighting(false);
    model->setUseCulling(false);

    return model;
}

void AnimationTest3::initialize()
{
    totalTime = 1.0f;
    currentTime = 0.0f;

    numberOfTransforms = 11;
    float minAngle = 0.0f;
    float maxAngle = SIMD_HALF_PI;
    float deltaAngle = (maxAngle - minAngle) / (float) numberOfTransforms;
    btVector3 axisOfRotation(0.0f, 0.0f, 1.0f);   // z-axis
    transforms = new float*[numberOfTransforms];
    for (int i = 0; i < numberOfTransforms; ++i)
    {
        transforms[i] = new float[16];
        // Rotation about z-axis
        btQuaternion rotation;
        float currentAngle = deltaAngle * (float) i;
        rotation.setRotation(axisOfRotation, currentAngle);
        btTransform transform;
        transform.setIdentity();
        transform.setRotation(rotation);
        transform.getOpenGLMatrix(transforms[i]);
        transforms[i][15] = 1.0f;
    }

    resultTransformation = new float[16];

}

void AnimationTest3::update(float deltaSeconds)
{
    if (!wasInitialized)
    {
        initialize();
        wasInitialized = true;
    }
    currentTime += deltaSeconds;
    if (currentTime >= totalTime)
    {
        currentTime = 0.0f;
    }

    float ratio = currentTime / totalTime;

    int currentFrame = (int) (ratio * (numberOfTransforms - 1.0f));

    float perFrameTime = totalTime / (float)(numberOfTransforms - 1);
    float frameStartTime = currentFrame * perFrameTime;
    float frameTimeRatio = (currentTime - frameStartTime) / perFrameTime;

    for (int i = 0; i < 16; ++i)
    {
        resultTransformation[i] = frameTimeRatio * transforms[currentFrame + 1][i] 
                                  + (1.0f - frameTimeRatio) * transforms[currentFrame][i];
    }
}

void AnimationTest3::draw(Graphics *graphics, bool picking)
{
    graphics->applyTransform(resultTransformation);
    if (texture)
    {
        texture->activate();
        if (!graphics->usingDirect3D())
            graphics->setColor(1, 1, 1, 1);
    }

    // TODO: The mesh has a texture, so we should use that one.
    mesh->draw(graphics, picking);

    if (texture)
    {
        texture->deactivate();
    }
}