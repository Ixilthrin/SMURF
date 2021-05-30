#ifndef GENG_DAE_VERTICES_NODE
#define GENG_DAE_VERTICES_NODE

#include "DaeInputNode.h"
#include "utXMLParser.h"
#include <string>

// Reflects the "vertices" node
class DaeVerticesNode
{
public:
    DaeVerticesNode(XMLNode *node);
    ~DaeVerticesNode();
    std::string getId() { return ident; }
    DaeInputNode *getInput() { return input; }
private:
    std::string ident;
    DaeInputNode *input;
};

#endif
