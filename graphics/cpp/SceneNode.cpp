#include "SceneNode.h"

#include "Drawable.h"
#include "GraphicsSystem.h"
#include "Graphics.h"
#include "Scene.h"
#include "util.h"

SceneNode::SceneNode() : colorId(0), drawable(0), texture(0), currentTime(0), scale(1.0)
{
    worldMatrix[0] = 1;
    worldMatrix[1] = 0;
    worldMatrix[2] = 0;
    worldMatrix[3] = 0;

    worldMatrix[4] = 0;
    worldMatrix[5] = 1;
    worldMatrix[6] = 0;
    worldMatrix[7] = 0;

    worldMatrix[8] = 0;
    worldMatrix[9] = 0;
    worldMatrix[10] = 1;
    worldMatrix[11] = 0;

    worldMatrix[12] = 0;
    worldMatrix[13] = 0;
    worldMatrix[14] = 0;
    worldMatrix[15] = 1;
}

void SceneNode::setTexture(const std::string & fileName)
{
    texture = SceneTexture::load(fileName);
}

void SceneNode::setTotalAnimationTime(float time)
{
    totalTime = time;
}

void SceneNode::update(float deltaSeconds)
{
    // This is for the animation, if this scene node has one.
    currentTime += deltaSeconds;
    if (currentTime >= totalTime)
    {
        currentTime = 0.0f;
    }
    drawable->update(deltaSeconds);
}

void SceneNode::draw(Graphics * graphics, bool picking)
{
    if (texture)
    {
        drawable->setTexture(texture->getTextureReference());
    }
    graphics->disableMode(Graphics::CULLING);
    drawable->setTotalAnimationTime(totalTime);
    drawable->setCurrentAnimationTime(currentTime);
    graphics->pushMatrix();
	 
	float finalMatrix[16];
	for (int i = 0; i < 16; i++) {
		finalMatrix[i] = worldMatrix[i];
	}
	finalMatrix[0] = finalMatrix[0] * scale;
	finalMatrix[1] = finalMatrix[1] * scale;
	finalMatrix[2] = finalMatrix[2] * scale;
	finalMatrix[4] = finalMatrix[4] * scale;
	finalMatrix[5] = finalMatrix[5] * scale;
	finalMatrix[6] = finalMatrix[6] * scale;
	finalMatrix[8] = finalMatrix[8] * scale;
	finalMatrix[9] = finalMatrix[9] * scale;
	finalMatrix[10] = finalMatrix[10] * scale;
    graphics->applyTransform(finalMatrix);
	if (picking)
	{
        graphics->setColor(colorId);
	}
	drawable->setWorldMatrix(finalMatrix);
	drawable->draw(graphics, picking);
	graphics->popMatrix();
}

