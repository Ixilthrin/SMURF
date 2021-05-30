#include "Model.h"

#include "DaeColladaNode.h"
#include "DaeFile.h"
#include "Graphics.h"
#include "GraphicsSystem.h"
#include "ModelCreator.h"
#include "ModelInfo.h"
#include "ModelInfoCreator.h"
#include <iostream>
#include "util.h"
#include "ShaderInfo.h"
#include "AnimationTest1.h"
#include "AnimationTest2.h"
#include "AnimationTest3.h"
#include "AnimationTest4.h"
#include "AnimationTest5.h"
#include "AnimationTest6.h"
#include "AnimationTest7.h"
#include "AnimationTest8.h"
#include "GeneralModel.h"
#include "ShaderTest1.h"
#include "TerrainSample1.h"
#include "XFile.h"

// TODO: Remove dependency on OpenGL from this class
#include "opengl_includes.h"
                                                                                
Model::Model()
: referenceCount(0), texture(0), vertexShader(0), fragmentShader(0),
  useCulling(true), useLighting(true), vertexType(-1), animation(0),
  isTextured(0)
{
}

void Model::acquireReference()
{
	++referenceCount;
}

void Model::releaseReference()
{
	--referenceCount;
	if (referenceCount == 0)
	{
		modelMap.erase(fileName);
		// texture->releaseReference();
		delete this;
		return;
	}
    else if (referenceCount < 0)
    {
        fatalError("Model released more times than acquired.");
    }
}

void Model::update(float deltaSeconds)
{
}

void Model::setTexture(TextureReference *texRef)
{
    if (fragmentShader && isTextured)
    {
        GraphicsSystem *gs = GraphicsSystem::getInstance();
        Graphics *graphics = gs->getGraphics();
        if (texRef && graphics->usingDirect3D())
        {
            fragmentShader->setUniformTexture("testTexture", texRef);
        }
    }
    texture = texRef;
}

void Model::draw(Graphics *graphics, bool picking)
{
    if (texture)
    {
        glEnable(GL_TEXTURE_2D);  // TODO: refactor this using Graphics class.
        texture->activate();
    }
    else
    {
        glDisable(GL_TEXTURE_2D);  // TODO: Use Graphics object
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
    glEnable(GL_TEXTURE_2D);  // TODO: refactor this using Graphics class.
//    graphics->enableMode(Graphics::TEXTURE_2D);
}

std::map<std::string, Model *> Model::modelMap;

Model * Model::create(const std::string & fileName)
{
    int daePos = fileName.rfind(std::string(".dae"));
    int xPos = fileName.rfind(std::string(".x"));

	Model * model = 0;
	if (modelMap.count(fileName) > 0)
	{
		model = modelMap[fileName];
	}
    else if (fileName == std::string("animation_test_1"))
    {
        model = AnimationTest1::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_2"))
    {
        model = AnimationTest2::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_3"))
    {
        model = AnimationTest3::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_4"))
    {
        model = AnimationTest4::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_5"))
    {
        model = AnimationTest5::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_6"))
    {
        model = AnimationTest6::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_7"))
    {
        model = AnimationTest7::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("animation_test_8"))
    {
        model = AnimationTest8::create();

		modelMap[fileName] = model;
    }
    else if (fileName.substr(0, 13) == std::string("general_model"))
    {
        std::string modelName = fileName.substr(14, fileName.size() - 14);
        model = GeneralModel::create(modelName);

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("shader_test_1"))
    {
        model = ShaderTest1::create();

		modelMap[fileName] = model;
    }
    else if (fileName == std::string("terrain_sample_1"))
    {
        model = TerrainSample1::create();

		modelMap[fileName] = model;
    }
    else if (daePos > 0)
	{
		DaeFile daeFile;
		daeFile.load(std::string("models/") + fileName);
        DaeColladaNode *colladaNode = daeFile.getColladaNode();
        ModelInfoCreator infoCreator(colladaNode);
        ModelInfo *modelInfo = infoCreator.getModelInfo();
        if (!modelInfo)
        {
            fatalError("Failed to create ModelInfo object: " + fileName);
        }
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        ModelCreator *creator = graphics->getModelCreator();
        model = creator->createModel(modelInfo);

		modelMap[fileName] = model;
	}
    else if (xPos > 0)
    {
        XFile *xFile = new XFile(std::string("models/") + fileName);
        ModelInfoCreator infoCreator(xFile);
        ModelInfo *modelInfo = infoCreator.getModelInfo();
        if (!modelInfo)
        {
            fatalError("Failed to create ModelInfo object: " + fileName);
        }
        Graphics *graphics = GraphicsSystem::getInstance()->getGraphics();
        ModelCreator *creator = graphics->getModelCreator();
        model = creator->createModel(modelInfo);

		modelMap[fileName] = model;
    }
	++model->referenceCount;
	return model;
}
