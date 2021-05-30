#include "DaeVisualSceneNode.h"

DaeVisualSceneNode::DaeVisualSceneNode(XMLNode *node)
: ident(), sceneNode(0)
{ 
    ident = std::string(node->getAttribute("id", 0));
    int numSceneNodes = node->nChildNode("node");
    if (numSceneNodes == 1)
    {
        XMLNode xmlSceneNode = node->getChildNode("node", 0);
        sceneNode = new DaeSceneNode(&xmlSceneNode);
    }
    else
    {
        printf("expecting 1 scene node, got %d\n", numSceneNodes);
    }
}

DaeVisualSceneNode::~DaeVisualSceneNode()
{
    if (sceneNode)
    {
        delete sceneNode;
    }
}
