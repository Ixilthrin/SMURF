#ifndef GENG_DAE_LIBRARY_MATERIALS_NODE
#define GENG_DAE_LIBRARY_MATERIALS_NODE

#include "DaeMaterialNode.h"
#include "utXMLParser.h"
#include <vector>

// Reflects the "library_materials" node
class DaeLibraryMaterialsNode
    {
    public:
        DaeLibraryMaterialsNode(XMLNode *node);
        ~DaeLibraryMaterialsNode();
        std::vector<DaeMaterialNode *> getMaterials() { return materials; }
    private:
        std::vector<DaeMaterialNode *> materials;
    };

#endif
