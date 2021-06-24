#ifndef GENG_UTIL_H
#define GENG_UTIL_H

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

//struct XMLNode;

// logging
#ifdef _DEBUG
#define LOG(msg) std::cout << msg << std::endl;
#else
#define LOG(msg)
#endif

// error reporting

void fatalError(const std::string & message);
//void checkOpenGlError(const std::string & message = std::string());
//void clearOpenGlError(unsigned int errorCodeToClear);
void assertFileExists(const std::string & fileName);

// math functions

//#define DEGREES_TO_RADIANS 3.14159265f / 180
#define DEGS_TO_RADS(degrees) degrees * 3.14159265f / 180.0f
float gengsin(float degrees);
float gengcos(float degrees);
float gengtan(float degrees);

// string functions

//float stringToFloat(const std::string & floatString, const std::string & onErrorString = "");

// xml functions


//XMLNode parseXmlFile(const std::string & xmlFile, const std::string & rootNodeName);
//float extractFloat(const XMLNode & node);

#endif
