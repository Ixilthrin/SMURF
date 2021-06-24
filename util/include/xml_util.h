#ifndef GENG_XML_UTIL_H
#define GENG_XML_UTIL_H

#include <string>
//#include <stdio.h>
//#include <stdlib.h>

struct XMLNode;

float stringToFloat(const std::string & floatString, const std::string & onErrorString = "");

XMLNode parseXmlFile(const std::string & xmlFile, const std::string & rootNodeName);
float extractFloat(const XMLNode & node);

#endif
