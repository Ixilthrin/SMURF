#include "AnimationTest1.h"
#include "ModelInfo.h"
#include "ModelCreator.h"
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "LinearMath/btTransform.h"
#include <math.h>

AnimationTest1::AnimationTest1()
: Model(), wasInitialized(0)
{
}

Model *AnimationTest1::create()
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
    modelInfo->setFileName("animation_test_1");
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

void AnimationTest1::initialize()
{
    totalTime = 5.0f;
    currentTime = 0.0f;

    T0 = new float[16];
    T1 = new float[16];
    resultT = new float[16];


    // Identity
    btTransform transform1;
    transform1.setIdentity();
    transform1.getOpenGLMatrix(T0);
    T0[15] = 1.0f;
    /*
    T0[0] = 1.0f;
    T0[1] = 0.0f;
    T0[2] = 0.0f;
    T0[3] = 0.0f;
    T0[4] = 0.0f;
    T0[5] = 1.0f;
    T0[6] = 0.0f;
    T0[7] = 0.0f;
    T0[8] = 0.0f;
    T0[9] = 0.0f;
    T0[10] = 1.0f;
    T0[11] = 0.0f;
    T0[12] = 0.0f;
    T0[13] = 0.0f;
    T0[14] = 0.0f;
    T0[15] = 1.0f;
    */
    

    // Rotation about z-axis by 180 degrees.
    btVector3 axisOfRotation(0.0f, 0.0f, 1.0f);   // z-axis
    float angle = SIMD_HALF_PI;
    btQuaternion rotation;
    rotation.setRotation(axisOfRotation, angle);
    btTransform transform2;
    transform2.setIdentity();
    transform2.setRotation(rotation);
    transform2.getOpenGLMatrix(T1);
    T1[15] = 1.0f;
    /*
    T1[0] = 0.0f;
    T1[1] = 1.0f;
    T1[2] = 0.0f;
    T1[3] = 0.0f;
    T1[4] = -1.0f;
    T1[5] = 0.0f;
    T1[6] = 0.0f;
    T1[7] = 0.0f;
    T1[8] = 0.0f;
    T1[9] = 0.0f;
    T1[10] = 1.0f;
    T1[11] = 0.0f;
    T1[12] = 0.0f;
    T1[13] = 0.0f;
    T1[14] = 0.0f;
    T1[15] = 1.0f;
    */

}

void AnimationTest1::update(float deltaSeconds)
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
    for (int i = 0; i < 16; ++i)
    {
        resultT[i] = ratio * T1[i] + (1.0f - ratio) * T0[i];
    }
}

void AnimationTest1::draw(Graphics *graphics, bool picking)
{
    graphics->applyTransform(resultT);
    if (texture && !picking)
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