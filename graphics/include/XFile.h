#ifndef __GENG_X_FILE_H__
#define __GENG_X_FILE_H__

#include "KeyFrameAnimation.h"

#include <fstream>
#include <string>
#include <stack>
#include <vector>

/**
 *  An Entry in an x file
 */
struct XFileObject
{
    XFileObject() : parent(0) {}
    std::string type;
    std::string name;
    std::vector<XFileObject *> children;
    std::vector<std::string> strings;
    std::vector<float> numbers;

    XFileObject *parent;  // 0 if no parent
};

class XFileObjectTypeCollector
{
public:
    XFileObjectTypeCollector(std::vector<XFileObject *> theRootObjects);
    std::vector<XFileObject *> getMatchingObjects(std::string theType);
private:
    void visit(XFileObject *object);
    std::string type;
    std::vector<XFileObject *> rootObjects;
    std::vector<XFileObject *> matchingObjects;
};

class XFile
{
public:
    XFile(std::string theFileName);
    std::vector<XFileObject *> getRootObjects() { return rootObjects; }
    std::string fileName;
    KeyframeAnimation *createKeyframeAnimation(int numberOfVertices);
    std::vector<XFileObject *> getXFileObjectsByType(std::string type);
    XFileObject *getRootFrame();
    MatrixArray getFrameTransformForObject(XFileObject *object);
    bool objectHasFrameTransform(XFileObject *object);
private:
    std::stack<XFileObject*> objectStack; // Used when reading hierarchy of frames, meshes, etc.
                                          // The top of the stack is the current context or parent object.
    std::vector<XFileObject *> rootObjects;
    std::fstream file;
    bool isAlpha(char c);
    bool isNumeric(char c);
    std::string readString();
    float readNumber();
    void readComment();
};

#endif
