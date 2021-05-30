#ifndef GENG_DAE_SOURCE_NODE
#define GENG_DAE_SOURCE_NODE

#include "DaeFloatArrayNode.h"
#include "utXMLParser.h"
#include <string>

// Reflects the "source" node
class DaeSourceNode
{
public:
    DaeSourceNode(XMLNode *node);
    ~DaeSourceNode();
    std::string getId() { return ident; }
    DaeFloatArrayNode *getFloatArrayNode() { return floatArrayNode; }
private:
    std::string ident;
    DaeFloatArrayNode *floatArrayNode;
};

#endif
