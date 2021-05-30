#ifndef GENG_DAE_LIBRARY_VISUAL_SCENES_NODE
#define GENG_DAE_LIBRARY_VISUAL_SCENES_NODE

#include "DaeVisualSceneNode.h"
#include "utXMLParser.h"

// Reflects the "library_visual_scenes" node
class DaeLibraryVisualScenesNode
{
public:
    DaeLibraryVisualScenesNode(XMLNode *node);
    ~DaeLibraryVisualScenesNode();
    DaeVisualSceneNode *getVisualScene() { return visualScene; }
private:
    DaeVisualSceneNode *visualScene;
};

#endif
