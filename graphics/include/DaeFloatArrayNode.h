#ifndef GENG_DAE_FLOAT_ARRAY_NODE
#define GENG_DAE_FLOAT_ARRAY_NODE

#include "utXMLParser.h"
#include <string>
#include <cstdio>


// Reflects the "float_array" node
class DaeFloatArrayNode
{
public:
    DaeFloatArrayNode(XMLNode *node);
    ~DaeFloatArrayNode();
    std::string getId() { return ident; }
    int getCount() { return count; }
    float *getValues() { return values; }
private:
    std::string ident;
    int count;
    float *values;
};

#endif
