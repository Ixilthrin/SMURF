#include "DaeGeometryNode.h"

DaeGeometryNode::DaeGeometryNode(XMLNode *node)
: ident(), name()
{
    ident = std::string(node->getAttribute("id", 0));
    //printf("geometry id = %s\n", id.c_str());
    name = std::string(node->getAttribute("name", 0));
    //printf("geometry name = %s\n", name.c_str());
    
    int numMeshNodes = node->nChildNode("mesh");
    //printf("geometry node has %d meshes\n", numMeshNodes);
    for (int i = 0; i < numMeshNodes; i++)
    {
        XMLNode meshNode = node->getChildNode("mesh", i);
        DaeMeshNode *daeMeshNode = new DaeMeshNode(&meshNode);
        meshes.push_back(daeMeshNode);
    }
}

DaeGeometryNode::~DaeGeometryNode()
{
    meshes.clear();
}