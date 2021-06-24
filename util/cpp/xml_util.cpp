#include "xml_util.h"
#include "util.h"
//#include "SDL_opengl.h"
#include <fstream>
#include <sstream>
#include "utXMLParser.h"

float stringToFloat(const std::string & floatString, const std::string & onErrorLocation)
{
    float result = 0;
    std::istringstream iss(floatString);
    iss >> result;
    if (iss.fail())
    {
        fatalError("Failed to convert " + floatString + " to float.  Location: " + onErrorLocation);
    }
    return result;
}

// Returns root node of xml file.
XMLNode parseXmlFile(const std::string & xmlFile, const std::string & rootNodeName)
{
    assertFileExists(xmlFile);
    XMLNode rootNode = XMLNode::parseFile(xmlFile.c_str());
    if (rootNode.isEmpty())
    {
        fatalError("Bad XML file: " + xmlFile);
    }
    if (rootNodeName != rootNode.getName())
    {
        fatalError("Root element should be " + rootNodeName + " in file " + xmlFile);
    }
    return rootNode;
}

float extractFloat(const XMLNode & node)
{
    std::string value = node.getText();
    return stringToFloat(value);

}
