#include "DaeInputNode.h"

// negative offset and set_number indicates not defined in file
DaeInputNode::DaeInputNode(XMLNode *node)
: semantic(), source(), offset(-1), setNumber(-1)
{
    semantic = std::string(node->getAttribute("semantic", 0));
    source = std::string(node->getAttribute("source", 0));
    if (node->isAttributeSet("offset"))
    {
        std::string strOffset = std::string(node->getAttribute("offset", 0));
        offset = atoi(strOffset.c_str());
    }
    if (node->isAttributeSet("set"))
    {
        std::string strSet = std::string(node->getAttribute("set", 0));
        setNumber = atoi(strSet.c_str());
    }
}

DaeInputNode::~DaeInputNode()
{
}