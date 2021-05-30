#include "DaeInstanceEffectNode.h"

DaeInstanceEffectNode::DaeInstanceEffectNode(XMLNode *node)
: url()
{
    url = std::string(node->getAttribute("url", 0));
    //printf("url = %s\n", url.c_str());
}

DaeInstanceEffectNode::~DaeInstanceEffectNode()
{
}
