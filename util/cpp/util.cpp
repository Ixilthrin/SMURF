#include "util.h"
#include <fstream>
#include <sstream>
#include "utXMLParser.h"
#ifdef WIN32
#include <windows.h>
#endif

// error reporting

void fatalError(const std::string & message)
{
#ifdef WIN32
	MessageBoxA(NULL, message.c_str(), "ERROR", MB_OK | MB_ICONEXCLAMATION);
#else
	std::cerr << message << std::endl;
	std::cerr << "Press any key to terminate..." << std::endl;
	std::cin.get();
#endif
	exit(1);
}

void assertFileExists(const std::string & fileName)
{
    std::ifstream file(fileName.c_str(), std::ifstream::in);
    if (file.fail()) 
    {
        fatalError(fileName + " doesn't exist.");
    }
    file.close();
}

// math functions

float gengsin(float degrees) 
{ 
	return static_cast<float>(sin(DEGS_TO_RADS(degrees))); 
}

float gengcos(float degrees) 
{ 
	return static_cast<float>(cos(DEGS_TO_RADS(degrees))); 
}

float gengtan(float degrees) 
{ 
	return static_cast<float>(tan(DEGS_TO_RADS(degrees))); 
}

// string functions

//float stringToFloat(const std::string & floatString, const std::string & onErrorLocation)
//{
//    float result = 0;
//    std::istringstream iss(floatString);
//    iss >> result;
//    if (iss.fail())
//    {
//        fatalError("Failed to convert " + floatString + " to float.  Location: " + onErrorLocation);
//    }
//    return result;
//}

// xml functions

// Returns root node of xml file.
//XMLNode parseXmlFile(const std::string & xmlFile, const std::string & rootNodeName)
//{
//    assertFileExists(xmlFile);
//    XMLNode rootNode = XMLNode::parseFile(xmlFile.c_str());
//    if (rootNode.isEmpty())
//    {
//        fatalError("Bad XML file: " + xmlFile);
//    }
//    if (rootNodeName != rootNode.getName())
//    {
//        fatalError("Root element should be " + rootNodeName + " in file " + xmlFile);
//    }
//    return rootNode;
//}
//
//float extractFloat(const XMLNode & node)
//{
//    std::string value = node.getText();
//    return stringToFloat(value);
//
//}
