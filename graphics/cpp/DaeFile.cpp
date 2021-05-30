#include "DaeFile.h"
#include "Graphics.h"
#include "GraphicsSystem.h"
#include "VertexBufferReference.h"
#include "VertexBufferInfo.h"
#include "IndexBufferReference.h"
#include "IndexBufferInfo.h"
#include "Vertex2.h"
#include "util.h"

DaeFile::DaeFile() 
: colladaNode(0), theFileName()
{
}

DaeFile::~DaeFile()
{
    if (colladaNode)
    {
        delete colladaNode;
    }
}

void DaeFile::load(std::string fileName)
{
    assertFileExists(fileName);
    theFileName = fileName;
    XMLNode topNode = XMLNode::parseFile(fileName.c_str());
//    printf("parsed %s\n", fileName.c_str());
	LOG("parsed " + fileName);
    // Process the "Collada" element.
    XMLNode xmlColladaNode = topNode.getChildNode();
    if (std::string(xmlColladaNode.getName()) != std::string("COLLADA"))
    {
        fatalError(std::string("Expected COLLADA node, got ") + xmlColladaNode.getName());
    }
    colladaNode = new DaeColladaNode(&xmlColladaNode);
    colladaNode->setFileName(fileName);
    
	//Core::getInstance()->fatalError("Failed to load file: " + fileName);
}

/**
 *   Description: This is a utility function to take a string containing many
 *                delimited values and convert it to a set of strings.
 *   @delimiter The character representing the delimiter, usually a space
 *   @theString  The string to tokenize
 */
std::vector<std::string> DaeFile::tokenizeString(char delimiter, std::string theString)
{
    std::vector<std::string> tokenizedStrings;
    int lastPosition = theString.length();
    int currentPosition = 0;
    while (currentPosition <= lastPosition)
    {
        while (theString[currentPosition] == delimiter)
        {
            currentPosition++;
        }
        int nextDelimiterPosition = theString.find(delimiter, currentPosition + 1);
        if (nextDelimiterPosition <= currentPosition)
        {
            std::string newString = theString.substr(currentPosition, lastPosition - currentPosition);
            tokenizedStrings.push_back(newString);
            currentPosition = lastPosition + 1;
        }
        else
        {
            std::string newString = theString.substr(currentPosition, nextDelimiterPosition - currentPosition);
            tokenizedStrings.push_back(newString);
            currentPosition = nextDelimiterPosition;
        }
        currentPosition++;
    }
    return tokenizedStrings;
}
