#ifndef GENG_SCENE_NODE_H
#define GENG_SCENE_NODE_H

#include "SceneTexture.h"

class Drawable;
class Graphics;
class GraphicsSystem;

class SceneNode
{
public:
    SceneNode();
    Drawable * getDrawable() const { return drawable; }
    void setColorId(unsigned int colorId) { this->colorId = colorId; }
    unsigned int getColorId() const { return colorId; }
    void draw(Graphics * graphics, bool picking);
    void update(float deltaTime);
    float * getWorldMatrixPtr() { return worldMatrix; }
    void setTexture(const std::string & fileName);
    void setTotalAnimationTime(float time);
    void setDrawable(Drawable *theDrawable) { drawable = theDrawable; }
	void setScale(float s) { scale = s;}

private:
    ~SceneNode() { }

    float worldMatrix[16]; // 4x4 transformation in OpenGL format (column-major order)
    Drawable * drawable;
    SceneTexture *texture;
    unsigned int colorId;
	float scale;

    // Animation
    float totalTime;
    float currentTime;
    
    friend class GraphicsSystem;
};

#endif