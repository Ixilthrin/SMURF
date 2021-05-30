#include "DaeInstanceGeometryNode.h"

DaeInstanceGeometryNode::DaeInstanceGeometryNode(XMLNode *node)
: url()
{
    url = std::string(node->getAttribute("url", 0));
    //printf("instance geometry url is %s\n", url.c_str());
}

DaeInstanceGeometryNode::~DaeInstanceGeometryNode()
{
}