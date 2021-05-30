#ifndef GENG_DAE_GEOMETRY_NODE
#define GENG_DAE_GEOMETRY_NODE

#include "DaeMeshNode.h"
#include "utXMLParser.h"
#include <string>
#include <vector>

// Reflects the "geometry" node
class DaeGeometryNode
{
public:
    DaeGeometryNode(XMLNode *node);
    ~DaeGeometryNode();
    
    std::vector<DaeMeshNode *> getMeshes() { return meshes; }
    std::string getId() { return ident; }
    std::string getName() { return name; }
private:
    std::vector<DaeMeshNode *> meshes;
    std::string ident;
    std::string name;
};

#endif
