#include "DaeFloatArrayNode.h"
#include "DaeFile.h"

DaeFloatArrayNode::DaeFloatArrayNode(XMLNode *node)
: ident(), count(0), values(0)
{
    ident = std::string(node->getAttribute("id", 0));
    //printf("float array id = %s\n", id.c_str());
    std::string strCount = std::string(node->getAttribute("count", 0));
    //printf("source count = %s\n", strCount.c_str());
    count = atoi(strCount.c_str());
    if (count < 1)
    {
        printf("float array has no values.\n");
        return;
    }
    std::string strValues = node->getText();
    std::vector<std::string> vectorValues = DaeFile::tokenizeString(' ', strValues);
    //printf("vector size %d\n", vectorValues.size());
    values = new float[vectorValues.size()];
    int index = 0;
    std::vector<std::string>::iterator iter = vectorValues.begin();
    std::vector<std::string>::iterator end = vectorValues.end();
    for (; iter != end; iter++)
    {
        //printf("%s ", (*iter).c_str());
        values[index] = static_cast<float>(atof((*iter).c_str()));
        index++;
    }
    //printf("\n");
}

DaeFloatArrayNode::~DaeFloatArrayNode()
{
    // Don't delete 'values' since it is passed to the Model.
}
