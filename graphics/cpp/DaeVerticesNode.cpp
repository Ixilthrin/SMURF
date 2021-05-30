#include "DaeVerticesNode.h"

DaeVerticesNode::DaeVerticesNode(XMLNode *node)
: ident(), input(0)
{
    // Process the "input" node
    int numInputNodes = node->nChildNode("input");
    //printf("vertices node has %d input node\n", numInputNodes);
    if (numInputNodes == 1)
    {
        XMLNode xmlInputNode = node->getChildNode("input", 0);
        input = new DaeInputNode(&xmlInputNode);
    }
    else
    {
        printf("vertices expected 1 input node, got %d\n", numInputNodes);
    }
}

DaeVerticesNode::~DaeVerticesNode()
{
    if (input)
    {
        delete input;
    }
}