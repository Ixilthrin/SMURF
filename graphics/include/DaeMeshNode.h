#ifndef GENG_DAE_MESH_NODE
#define GENG_DAE_MESH_NODE

#include "DaeSourceNode.h"
#include "DaeTrianglesNode.h"
#include "DaeVerticesNode.h"
#include "utXMLParser.h"
#include <vector>

// Reflects the "mesh" node
class DaeMeshNode
{
public:
    DaeMeshNode(XMLNode *node);
    ~DaeMeshNode();
    std::vector<DaeSourceNode *> getSources() { return sources; }
    std::vector<DaeVerticesNode *> getVertices() { return vertices; }
    std::vector<DaeTrianglesNode *> getTriangles() { return triangles; }
private:
    std::vector<DaeSourceNode *> sources;
    std::vector<DaeVerticesNode *> vertices;
    std::vector<DaeTrianglesNode *> triangles;
};

#endif
