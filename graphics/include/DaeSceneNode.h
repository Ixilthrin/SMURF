#ifndef GENG_DAE_SCENE_NODE
#define GENG_DAE_SCENE_NODE

#include "DaeInstanceGeometryNode.h"
#include "utXMLParser.h"
#include <string>

// Reflects the XML node named "node" under "visual_scene"
// Can contain a DeaSceneNode
class DaeSceneNode
{
public:
    DaeSceneNode(XMLNode *node);
    ~DaeSceneNode();
    std::string getId() { return ident; }
    std::string getName() { return name; }
    DaeSceneNode *getChild() { return child; }
    DaeInstanceGeometryNode *getInstanceGeometry() { return instanceGeometry; }
private:
    std::string ident;
    std::string name;
    DaeSceneNode *child;
    DaeInstanceGeometryNode *instanceGeometry;
};

#endif
