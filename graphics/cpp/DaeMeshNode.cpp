#include "DaeMeshNode.h"

DaeMeshNode::DaeMeshNode(XMLNode *node)
: sources(), vertices(), triangles()
{ 
    int numSourceNodes = node->nChildNode("source");
    //printf("mesh node has %d sources\n", numSourceNodes);
    for (int i = 0; i < numSourceNodes; i++)
    {
        XMLNode sourceNode = node->getChildNode("source", i);
        DaeSourceNode *daeSourceNode = new DaeSourceNode(&sourceNode);
        sources.push_back(daeSourceNode);
    }
    int numTriangleNodes = node->nChildNode("triangles");
    //printf("mesh node has %d triangles\n", numTriangleNodes);
    for (int i = 0; i < numTriangleNodes; i++)
    {
        XMLNode triangleNode = node->getChildNode("triangles", i);
        DaeTrianglesNode *daeTriangleNode = new DaeTrianglesNode(&triangleNode);
        triangles.push_back(daeTriangleNode);
    }
}

DaeMeshNode::~DaeMeshNode()
{
    sources.clear();
    vertices.clear();
    triangles.clear();
}