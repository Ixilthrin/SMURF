#include "AnimationTest2.h"
#include "ModelInfo.h"
#include "ModelCreator.h"
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "LinearMath/btTransform.h"

AnimationTest2::AnimationTest2()
: Model(), wasInitialized(0)
{
}

Model *AnimationTest2::create()
{
    float vertexPositions[] =
    {
        -1.0f, 0.0f, 0.0f,
        0.0f, .2f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, -.2f, 0.0f
    };

    float texCoords[] =
    { 0.0f,0.5f, .5f, 0.0f, 1.0f, .5f, .5f, 1.0f };

    unsigned short indices[] = { 0, 1, 2, 3, 0, 2 };

    ModelInfo *modelInfo = new ModelInfo();
    modelInfo->setFileName("animation_test_2");
    modelInfo->hasColorValues = false;
    modelInfo->hasTexture = true;
    modelInfo->textureCoords = texCoords;
    modelInfo->indexCount = 6;
    modelInfo->indices = indices;
    modelInfo->vertexCount = 4;
    modelInfo->vertexPositions = vertexPositions;
    modelInfo->textureFile = std::string("images/radar.bmp");

    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    model->setUseLighting(false);
    model->setUseCulling(false);

    return model;
}

void AnimationTest2::initialize()
{

    timeStep1 = 1.0f;
    timeStep2 = 2.0f;

    currentTime = 0.0f;

    T0 = new float[16];
    T1 = new float[16];
    T2 = new float[16];
    resultT = new float[16];

    // Identity
    btTransform transform1;
    transform1.setIdentity();
    transform1.getOpenGLMatrix(T0);
    T0[15] = 1.0f;
    /*
    T0[15] = 1.0f;
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

    // Translate up along y-axis
    btVector3 translation1(0.0f, 2.0f, 1.0f);   // translate to (0, 2, 0)
    btTransform transform2;
    transform2.setIdentity();
    transform2.setOrigin(translation1);
    transform2.getOpenGLMatrix(T1);
    T1[15] = 1.0f;
    /*
    T1[0] = 1.0f;
    T1[1] = 0.0f;
    T1[2] = 0.0f;
    T1[3] = 0.0f;
    T1[4] = 0.0f;
    T1[5] = 1.0f;
    T1[6] = 0.0f;
    T1[7] = 0.0f;
    T1[8] = 0.0f;
    T1[9] = 0.0f;
    T1[10] = 1.0f;
    T1[11] = 0.0f;
    T1[12] = 0.0f;
    T1[13] = 2.0f;
    T1[14] = 0.0f;
    T1[15] = 1.0f;
    */

    // Translate down along y-axis
    btTransform transform3;
    transform3.setIdentity();
    transform3.getOpenGLMatrix(T2);
    T2[15] = 1.0f;
    /*
    T2[0] = 1.0f;
    T2[1] = 0.0f;
    T2[2] = 0.0f;
    T2[3] = 0.0f;
    T2[4] = 0.0f;
    T2[5] = 1.0f;
    T2[6] = 0.0f;
    T2[7] = 0.0f;
    T2[8] = 0.0f;
    T2[9] = 0.0f;
    T2[10] = 1.0f;
    T2[11] = 0.0f;
    T2[12] = 0.0f;
    T2[13] = 0.0f;
    T2[14] = 0.0f;
    T2[15] = 1.0f;
    */

}

void AnimationTest2::update(float deltaSeconds)
{
    if (!wasInitialized)
    {
        initialize();
        wasInitialized = true;
    }
    currentTime += deltaSeconds;



    if (currentTime >= timeStep2)
    {
        currentTime = currentTime - timeStep2;
    }

    float *key1 = 0;
    float *key2 = 0;
    float ratio = 0;

    if (currentTime < timeStep1)
    {
        key1 = T0;
        key2 = T1;
        ratio = currentTime / timeStep1;
    }
    else
    {
        key1 = T1;
        key2 = T2;
        ratio = (currentTime - timeStep1) / (timeStep2 - timeStep1);
    }

    for (int i = 0; i < 16; ++i)
    {
        resultT[i] = ratio * key2[i] + (1.0f - ratio) * key1[i];
    }
}

void AnimationTest2::draw(Graphics *graphics, bool picking)
{
    graphics->setColor(1, 1, 1, 1);
    if (texture)
    {
        texture->activate();
    }

    // TODO: The mesh has a texture, so we should use that one.
    graphics->applyTransform(resultT);
    mesh->draw(graphics, picking);
/*
    VertexBufferReference *vertexBuffer = mesh->getVertexBuffer();
    IndexBufferReference *indexBuffer = mesh->getIndexBuffer();
    indexBuffer->activate();
    vertexBuffer->activate(picking);
    //int indexCount = indexBuffer->getIndexCount();
    //graphics->applyTransform(resultT);
    graphics->applyTransform(T0);
    graphics->drawIndexedTriangles(3, 3);
    graphics->applyTransform(resultT);
    graphics->drawIndexedTriangles(3, 0);
    vertexBuffer->deactivate();
    indexBuffer->deactivate();
*/

    if (texture)
    {
        texture->deactivate();
    }
}