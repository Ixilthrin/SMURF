#ifndef GENG_DAE_INSTANCE_GEOMETRY_NODE
#define GENG_DAE_INSTANCE_GEOMETRY_NODE

#include "utXMLParser.h"
#include <string>
#include <cstdio>



// Reflects the "instance_geometry" node
class DaeInstanceGeometryNode
{
public:
    DaeInstanceGeometryNode(XMLNode *node);
    ~DaeInstanceGeometryNode();
    std::string getUrl() { return url; }
private:
    std::string url;
};

#endif
