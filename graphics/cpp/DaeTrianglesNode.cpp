#include "DaeTrianglesNode.h"
#include "DaeFile.h"

DaeTrianglesNode::DaeTrianglesNode(XMLNode *node)
: material(), count(0), values(0), hasMaterial(false)
{
    if (node->isAttributeSet("material"))
    {
        hasMaterial = true;
        material = std::string(node->getAttribute("material", 0));
    }
    std::string strCount = std::string(node->getAttribute("count", 0));
    count = atoi(strCount.c_str());
    //printf("%d triangles\n", count);
    
    // Process the "input" nodes
    int numInputNodes = node->nChildNode("input");
    //printf("triangles node has %d input nodes\n", numInputNodes);
    for (int i = 0; i < numInputNodes; i++)
    {
        XMLNode xmlInputNode = node->getChildNode("input", i);
        DaeInputNode *input = new DaeInputNode(&xmlInputNode);
        inputs.push_back(input);
    }
    
    // Get the values between <p> and </p>
    int numPNodes = node->nChildNode("p");
    if (numPNodes == 1)
    {
        XMLNode pNode = node->getChildNode("p", 0);
        std::string strValues = std::string(pNode.getText());
        //printf("p node value: %s\n", strValues.c_str());
        std::vector<std::string> valueStrings = DaeFile::tokenizeString(' ', strValues);
        numberOfValues = valueStrings.size();
        values = new int[numberOfValues];
        std::vector<std::string>::iterator iter = valueStrings.begin();
        std::vector<std::string>::iterator end = valueStrings.end();
        int index = 0;
        for (; iter != end; iter++)
        {
            values[index] = atoi((*iter).c_str());
            index++;
        }
        //printf("number of values = %d\n", numberOfValues);
    }
    else
    {
        printf("triangles does not have exactly 1 <p> element as expected\n");
        return;
    }
    
}

DaeTrianglesNode::~DaeTrianglesNode()
{
    inputs.clear();
}