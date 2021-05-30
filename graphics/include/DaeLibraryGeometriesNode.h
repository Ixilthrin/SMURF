#ifndef GENG_DAE_LIBRARY_GEOMETRIES_NODE
#define GENG_DAE_LIBRARY_GEOMETRIES_NODE

#include "DaeGeometryNode.h"
#include "utXMLParser.h"
#include <vector>

// Reflects the "library_geometries node
class DaeLibraryGeometriesNode
{
public:
    DaeLibraryGeometriesNode(XMLNode *node);
    ~DaeLibraryGeometriesNode();
    std::vector<DaeGeometryNode *> getGeometries() { return geometries; }
private:
    std::vector<DaeGeometryNode *> geometries;
};

#endif
