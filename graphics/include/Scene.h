#ifndef GENG_SCENE_H
#define GENG_SCENE_H

#include <vector>
#include <map>

class GraphicsSystem;
class Graphics;
class SceneNode;

class Scene
{
public:
	Scene(Graphics * g);
	void update(float deltaSeconds);
	void draw();
	unsigned int pick(int screenX, int screenY);
    void addSceneNode(SceneNode * node) { sceneNodes.push_back(node); }

private:
//    typedef unsigned int COLOR_ID;
//    std::map<COLOR_ID, SceneNode *> colorIdMap;

	std::vector<SceneNode *> sceneNodes;
    Graphics * graphics;
    friend class GraphicsSystem;
};

#endif
