#include "DaeEffectNode.h"

DaeEffectNode::DaeEffectNode(XMLNode *node)
: ident(), name(), imageId()
{
    ident = std::string(node->getAttribute("id", 0));
    //printf("effect id = %s\n", id.c_str());
    name = std::string(node->getAttribute("name", 0));
    //printf("effect name = %s\n", name.c_str());
 
    // Drill down to get the image id
    int numNewParamNode = node->nChildNode("newparam");
    if (numNewParamNode == 1)
    {
        XMLNode *newParamNode = &(node->getChildNode(0));
        int numSurfaceNodes = newParamNode->nChildNode("surface");
        if (numSurfaceNodes == 1)
        {
            XMLNode *surfaceNode = &(newParamNode->getChildNode(0));
            int numInitFromNodes = surfaceNode->nChildNode("init_from");
            if (numInitFromNodes == 1)
            {
                XMLNode *initFromNode = &(surfaceNode->getChildNode(0));
                imageId = std::string(initFromNode->getText());
                //printf("image id for effect node is %s\n", imageId.c_str());
            }
        }
    }
}

DaeEffectNode::~DaeEffectNode()
{
}


