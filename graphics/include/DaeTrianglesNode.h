#ifndef GENG_DAE_TRIANGLES_NODE
#define GENG_DAE_TRIANGLES_NODE

#include "DaeInputNode.h"
#include "utXMLParser.h"
#include <string>
#include <vector>

// Reflects the "triangles" node
class DaeTrianglesNode
{
public:
    DaeTrianglesNode(XMLNode *node);
    ~DaeTrianglesNode();
    std::string getMaterial() { return material; }
    int getCount() { return count; }
    int getNumberOfValues() { return numberOfValues; }
    int *getValues() { return values; }
    std::vector<DaeInputNode *> getInputs() { return inputs; }
    bool getHasMaterial() { return hasMaterial; }
private:
    bool hasMaterial;
    std::string material;
    int count;
    int numberOfValues;
    int *values;
    std::vector<DaeInputNode *> inputs;
};

#endif
