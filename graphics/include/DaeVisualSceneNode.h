#ifndef GENG_DAE_VISUAL_SCENE_NODE
#define GENG_DAE_VISUAL_SCENE_NODE

#include "DaeSceneNode.h"
#include "utXMLParser.h"
#include <string>

// Reflects the "visual_scene" node
class DaeVisualSceneNode
{
public:
    DaeVisualSceneNode(XMLNode *node);
    ~DaeVisualSceneNode();
    std::string getId() { return ident; }
    DaeSceneNode *getSceneNode() { return sceneNode; }
private:
    std::string ident;
    DaeSceneNode *sceneNode;
};

#endif
