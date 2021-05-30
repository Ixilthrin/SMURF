#include "DaeImageNode.h"

DaeImageNode::DaeImageNode(XMLNode *node)
: ident(), name(), fileName()
{
    ident = std::string(node->getAttribute("id", 0));
    //printf("effect id = %s\n", id.c_str());
    name = std::string(node->getAttribute("name", 0));
    //printf("effect name = %s\n", name.c_str());
    
    int numInitFromNodes = node->nChildNode("init_from");
    if (numInitFromNodes == 1)
    {
        XMLNode *initFromNode = &(node->getChildNode(0));
        fileName = std::string(initFromNode->getText());
        //printf("image file name is %s\n", fileName.c_str());
    }
}

DaeImageNode::~DaeImageNode()
{
}