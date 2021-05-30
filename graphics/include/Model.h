#ifndef GENG_MODEL_H
#define GENG_MODEL_H

#include <map>
#include "Drawable.h"
#include "Graphics.h"
#include "Mesh.h"
#include "TextureReference.h"
#include "ShaderReference.h"
#include "KeyFrameAnimation.h"

class Graphics;
class D3DModelCreator;
class GLModelCreator;

class Model : public Drawable
{
public:
	static Model * create(const std::string & filename);
    virtual void acquireReference();
    virtual void releaseReference();
//	static void destroy(Model * model);  // TODO: delete this line or integrate it.
	virtual void update(float deltaSeconds);
	virtual void draw(Graphics *graphics, bool picking);
    void setFileName(std::string name) { fileName = name; }

    void setMesh(Mesh *theMesh) { mesh = theMesh; }
    void setTextureReference(TextureReference *ref) { texture = ref; }
    virtual void setTexture(TextureReference *texRef);
    void setVertexShader(ShaderReference *ref) { vertexShader = ref; }
    void setTotalAnimationTime(float time) { totalTime = time; }
    void setFragmentShader(ShaderReference *ref) { fragmentShader = ref; }
    void setUseLighting(bool value) { useLighting = value; }
    void setUseCulling(bool value) { useCulling = value; }

    void setVertexType(int type) { vertexType = type; }
    int getVertexType() { return vertexType; }
    void setKeyframeAnimation(KeyframeAnimation *theAnimation) { animation = theAnimation; }
    bool hasAnimation() { return animation != 0; }
    bool hasVertexShader() { return vertexShader != 0; }
    bool hasFragmentShader() { return fragmentShader != 0; }
    void setIsTextured(bool value) { isTextured = value; }

protected:
	Model();
	virtual ~Model() { }

    friend class D3DModelCreator;
    friend class GLModelCreator;
    friend class AnimationTest4;
    friend class AnimationTest5;
    friend class AnimationTest6;
    friend class AnimationTest7;
    friend class AnimationTest8;
    friend class GeneralModel;
    friend class ShaderTest1;
    friend class TerrainSample1;

	std::string fileName;
    bool useCulling;
    bool useLighting;

    Mesh *mesh;
    bool isTextured;
    TextureReference *texture;
    ShaderReference *vertexShader;
    ShaderReference *fragmentShader;

    float totalTime; // total animation time
    KeyframeAnimation *animation;

    int vertexType;  // e.g. VertexBufferInfo::VERTEX1

	static std::map<std::string, Model *> modelMap;
	int referenceCount;
};

#endif
