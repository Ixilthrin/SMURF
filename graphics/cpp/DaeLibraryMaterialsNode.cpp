#include "DaeLibraryMaterialsNode.h"

DaeLibraryMaterialsNode::DaeLibraryMaterialsNode(XMLNode *node)
: materials()
{
    // process the "material" elements
    int numMaterialElements = node->nChildNode("material");
    if (numMaterialElements <= 0)
    {
        printf("No material elements found in library_materials element.\n");
        return;
    }
    for (int i = 0; i < numMaterialElements; i++)
    {
        XMLNode child = node->getChildNode(i);
        materials.push_back(new DaeMaterialNode(&child));
    }
}

DaeLibraryMaterialsNode::~DaeLibraryMaterialsNode()
{
    materials.clear();
}
