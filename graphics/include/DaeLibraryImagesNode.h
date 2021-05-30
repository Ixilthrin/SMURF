#ifndef GENG_DAE_LIBRARY_IMAGES_NODE
#define GENG_DAE_LIBRARY_IMAGES_NODE

#include "DaeImageNode.h"
#include "utXMLParser.h"
#include <vector>

// Reflects the "library_images" node
class DaeLibraryImagesNode
    {
    public:
        DaeLibraryImagesNode(XMLNode *node);
        ~DaeLibraryImagesNode();
        std::vector<DaeImageNode *> getImages() { return images; }
    private:
        std::vector<DaeImageNode *> images;
    };

#endif