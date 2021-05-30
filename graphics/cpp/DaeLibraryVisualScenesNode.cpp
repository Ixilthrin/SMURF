#include "DaeLibraryVisualScenesNode.h"

DaeLibraryVisualScenesNode::DaeLibraryVisualScenesNode(XMLNode *node)
{
    // Process the "visual_scene" element.
    int numVisualScenesNodes = node->nChildNode("visual_scene");
    if (numVisualScenesNodes == 1)
    {
        //printf("found 1 visual scene node\n");
        XMLNode xmlVisualScene = node->getChildNode("visual_scene");
        visualScene = new DaeVisualSceneNode(&xmlVisualScene);
    }
    else
    {
        printf("Expected 1 visual_scene element, got %d\n", numVisualScenesNodes);
        return;
    }
}

DaeLibraryVisualScenesNode::~DaeLibraryVisualScenesNode()
{
}