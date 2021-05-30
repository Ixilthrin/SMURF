#include "TerrainSample1.h"
#include "ModelInfo.h"
#include "ModelCreator.h"
#include "GraphicsSystem.h"
#include "Mesh.h"
#include "LinearMath/btTransform.h"
#include <math.h>

// TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"

TerrainSample1::TerrainSample1()
: Model(), wasInitialized(0)
{
}

Model *TerrainSample1::create()
{
    float vertexPositions[] =
    {
        -50.0f, 0.0f, 50.0f,
        50.0f, 0.0f, 50.0f,
        50.0f, 0.0f, -50.0f,
        -50.0f, 0.0f, -50.0f,
    };

    float texCoords[] =
    { 1.0f,1.0f,  0.0f,1.0f, 0.0f,0.0f, 1.0f,0.0f };

    unsigned short indices[] = { 0, 2, 1, 3, 2, 0 };

    ModelInfo *modelInfo = new ModelInfo();
    modelInfo->setFileName("terrain_sample_1");
    modelInfo->hasColorValues = false;
    modelInfo->hasTexture = true;
    modelInfo->textureCoords = texCoords;
    modelInfo->indexCount = 6;
    modelInfo->indices = indices;
    modelInfo->vertexCount = 4;
    modelInfo->vertexPositions = vertexPositions;

    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    model->setUseLighting(false);
    model->setUseCulling(false);

    return model;
}

void TerrainSample1::initialize()
{
}

void TerrainSample1::update(float deltaSeconds)
{
    if (!wasInitialized)
    {
        initialize();
        wasInitialized = true;
    }
}

void TerrainSample1::draw(Graphics *graphics, bool picking)
{
    glEnable(GL_TEXTURE_2D);  // Use graphics object for this, not OpenGL directly.
    graphics->setColor(1, 1, 1, 1);

    // The texture is set in SceneNode.
    if (texture)
    {
        texture->activate();
    }

    mesh->draw(graphics, picking);

    if (texture)
    {
        texture->deactivate();
    }

    graphics->setColor(0, 0, 0, 0);
}