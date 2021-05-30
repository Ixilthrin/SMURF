#include "DaeLibraryGeometriesNode.h"

DaeLibraryGeometriesNode::DaeLibraryGeometriesNode(XMLNode *node)
: geometries()
{
    // process the "geometry" elements
    int numGeometryElements = node->nChildNode("geometry");
    if (numGeometryElements <= 0)
    {
        printf("No geometry elements found in library_geometries element.\n");
        return;
    }
    for (int i = 0; i < numGeometryElements; i++)
    {
        XMLNode child = node->getChildNode(i);
        geometries.push_back(new DaeGeometryNode(&child));
    }
}

DaeLibraryGeometriesNode::~DaeLibraryGeometriesNode()
{
    geometries.clear();
}