#include "DaeLibraryImagesNode.h"

DaeLibraryImagesNode::DaeLibraryImagesNode(XMLNode *node)
: images()
{
    // process the "images" elements
    int numImageElements = node->nChildNode("image");
    if (numImageElements <= 0)
    {
        printf("No image elements found in library_images element.\n");
        return;
    }
    for (int i = 0; i < numImageElements; i++)
    {
        XMLNode child = node->getChildNode(i);
        images.push_back(new DaeImageNode(&child));
    }
}

DaeLibraryImagesNode::~DaeLibraryImagesNode()
{
    images.clear();
}