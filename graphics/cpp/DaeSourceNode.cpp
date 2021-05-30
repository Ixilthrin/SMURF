#include "DaeSourceNode.h"

DaeSourceNode::DaeSourceNode(XMLNode *node)
: ident(), floatArrayNode(0)
{
    ident = std::string(node->getAttribute("id", 0));
    //printf("source id = %s\n", id.c_str());    
    int numFloatArrayNodes = node->nChildNode("float_array");
    //printf("source node has %d float arrays\n", numFloatArrayNodes);
    if (numFloatArrayNodes == 1)
    {
        XMLNode xmlFloatArrayNode = node->getChildNode("float_array", 0);
        floatArrayNode = new DaeFloatArrayNode(&xmlFloatArrayNode);
    }
}

DaeSourceNode::~DaeSourceNode()
{
    if (floatArrayNode)
    {
        delete floatArrayNode;
    }
}