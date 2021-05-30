#include "ShaderTest1.h"
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

static bool testGLSL = false;
static float totalTestTime = 0.0f;

ShaderTest1::ShaderTest1()
: Model(), wasInitialized(0)
{
}

Model *ShaderTest1::create()
{
    float vertexPositions[] =
    {
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        -1.0f,-1.0f, 0.0f,
        1.0f,-1.0f, 0.0f
    };
    float colors[] =
    { 1,0,0, 0,1,0, 0,0,1, 1,0,1 };  // RGB format x 4 vertices

    float texCoords[] =
    { 0.0f,0.0f,  1.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f };

    unsigned short indices[] = { 0, 1, 2, 2, 1, 3 };

    ModelInfo *modelInfo = new ModelInfo();
    modelInfo->setFileName("shader_test_1");
    modelInfo->hasColorValues = true;
    modelInfo->colorValueCount = 4;
    modelInfo->colorValues = colors;
    modelInfo->hasTexture = true;
    modelInfo->textureCoords = texCoords;
    modelInfo->indexCount = 6;
    modelInfo->indices = indices;
    modelInfo->vertexCount = 4;
    modelInfo->vertexPositions = vertexPositions;
    modelInfo->textureFile = std::string("images/test.bmp");

    Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
    ModelCreator *creator = graphics->getModelCreator();
    Model *model = creator->createModel(modelInfo);

    ShaderInfo *vertexShaderInfo = 0;
    ShaderInfo *fragmentShaderInfo = 0;
    if (graphics->usingDirect3D())
    {
        vertexShaderInfo = new ShaderInfo(ShaderInfo::VERTEX_SHADER, 
            ShaderInfo::TEXT_FILE, std::string("shaders/vertex_shader.cgvert"), 
            model->getVertexType(), "shader_test_1_context");
        fragmentShaderInfo = new ShaderInfo(ShaderInfo::FRAGMENT_SHADER, 
            ShaderInfo::TEXT_FILE, std::string("shaders/pixel_shader.cgfrag"), 
            model->getVertexType(), "shader_test_1_context");
    }
    else if (testGLSL)
    {
        vertexShaderInfo = new ShaderInfo(ShaderInfo::VERTEX_SHADER, 
            //ShaderInfo::TEXT_FILE, std::string("shaders/test.glvert"), 
            ShaderInfo::TEXT_FILE, std::string("shaders/simple_bouncing.glvert"), 
            model->getVertexType(), "shader_test_1_context");
        fragmentShaderInfo = new ShaderInfo(ShaderInfo::FRAGMENT_SHADER, 
            ShaderInfo::TEXT_FILE, std::string("shaders/test.glfrag"), 
            model->getVertexType(), "shader_test_1_context");
    }
    else
    {
        vertexShaderInfo = new ShaderInfo(ShaderInfo::VERTEX_SHADER, 
            ShaderInfo::TEXT_FILE, std::string("shaders/vertex_shader_gl.cgvert"), 
            model->getVertexType(), "shader_test_1_context");
        fragmentShaderInfo = new ShaderInfo(ShaderInfo::FRAGMENT_SHADER, 
            ShaderInfo::TEXT_FILE, std::string("shaders/pixel_shader_gl.cgfrag"), 
            model->getVertexType(), "shader_test_1_context");
    }

    ShaderReference *vertexShader = graphics->createShader(vertexShaderInfo);

    if (vertexShader && !testGLSL)
    {
        vertexShader->bindParameterName("worldViewProj", ShaderInfo::MATRIX);
    }

    ShaderReference *fragmentShader = graphics->createShader(fragmentShaderInfo);

    if (fragmentShader)
    {
        if (graphics->usingDirect3D() && !testGLSL)
        {
            fragmentShader->bindParameterName("testTexture", ShaderInfo::SAMPLER_2D);
        }
    }


    model->setVertexShader(vertexShader);
    model->setFragmentShader(fragmentShader);
    model->useLighting = false;
    model->useCulling = false;

    delete vertexShaderInfo;
    delete fragmentShaderInfo;

    return model;
}

void ShaderTest1::initialize()
{
}

void ShaderTest1::update(float deltaSeconds)
{
    totalTestTime += deltaSeconds;
}

void ShaderTest1::draw(Graphics *graphics, bool picking)
{
    if (texture)
    {
        glEnable(GL_TEXTURE_2D);  // TODO: refactor this using Graphics class.
        //texture->activate();
    }
    else
    {
        glDisable(GL_TEXTURE_2D);  // TODO: Use Graphics object
    }
    if (vertexShader)
    {
        graphics->setShaderIsActive(true);
        if (!testGLSL)
        {
            float *worldViewProj = graphics->getModelViewProjectionMatrix();
            if (graphics->usingDirect3D())
            {
                vertexShader->setUniformParameter("worldViewProj", worldViewProj);
            }
            else
            {
                vertexShader->setUniformMatrix("worldViewProj", worldViewProj);
            }
            delete [] worldViewProj;
        }
        vertexShader->activate();

        // For GLSL, the parameters need to be bound just before being set.
        // This is different from Cg.
        if (testGLSL)
        {
            vertexShader->bindParameterName("time", ShaderInfo::FLOAT_VALUE);
            vertexShader->setUniformParameter("time", &totalTestTime);
            vertexShader->bindAttributeName("vel", ShaderInfo::FLOAT_VALUE);
            float vel = 0.0f;
            vertexShader->setAttribute("vel", &vel);
        }
    }
    if (fragmentShader)
    {
        graphics->setShaderIsActive(true);
        if (graphics->usingDirect3D() && !testGLSL)
        {
            fragmentShader->setUniformTexture("testTexture", texture);
        }
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

    if (texture)
    {
        texture->deactivate();
    }
    if (vertexShader)
    {
        vertexShader->deactivate();
    }
    if (fragmentShader)
    {
        fragmentShader->deactivate();
    }
    graphics->setShaderIsActive(false);
    glEnable(GL_TEXTURE_2D);  // TODO: refactor this using Graphics class.
    //    graphics->enableMode(Graphics::TEXTURE_2D);
}