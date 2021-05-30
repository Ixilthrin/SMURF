#ifndef GENG_DAE_INPUT_NODE
#define GENG_DAE_INPUT_NODE

#include "utXMLParser.h"
#include <string>
#include <cstdio>


// Reflects the "input" node
class DaeInputNode
{
public:
    DaeInputNode(XMLNode *node);
    ~DaeInputNode();
    std::string getSemantic() { return semantic; }
    std::string getSource() { return source; }
    int getOffset() { return offset; }
    int getSetNumber() { return setNumber; }
private:
    std::string semantic;
    std::string source;
    int offset;
    int setNumber;
};

#endif
