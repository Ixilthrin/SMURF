#include "DaeSceneNode.h"

DaeSceneNode::DaeSceneNode(XMLNode *node)
: ident(), name(), child(0)
{ 
    ident = std::string(node->getAttribute("id", 0));
    name = std::string(node->getAttribute("name", 0));
    
    int numSceneNodeChildren = node->nChildNode("node");
    if (numSceneNodeChildren == 1)
    {
        XMLNode xmlChildNode = node->getChildNode("node", 0);
        child = new DaeSceneNode(&xmlChildNode);
    }
    else if (numSceneNodeChildren > 1)
    {
        printf("expected only 1 scene node children, got %d\n", numSceneNodeChildren);
    }
    
    int numInstanceGeometries = node->nChildNode("instance_geometry");
    if (numInstanceGeometries == 1)
    {
        XMLNode xmlInstanceGeometry = node->getChildNode("instance_geometry", 0);
        instanceGeometry = new DaeInstanceGeometryNode(&xmlInstanceGeometry);
    }
    else if (numInstanceGeometries > 1)
    {
        printf("expected only 1 instance geometry, got %d\n", numInstanceGeometries);
    }
    
}

DaeSceneNode::~DaeSceneNode()
{
    if (child)
    {
        delete child;
    }
}