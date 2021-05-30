#ifndef GENG_DAE_IMAGE_NODE
#define GENG_DAE_IMAGE_NODE

#include "utXMLParser.h"
#include <string>
#include <vector>
#include <cstdio>


// Reflects the "image" node
class DaeImageNode
    {
    public:
        DaeImageNode(XMLNode *node);
        ~DaeImageNode();
        
        std::string getId() { return ident; }
        std::string getName() { return name; }
        std::string getFileName() { return fileName; }
    private:
        std::string ident;
        std::string name;
        std::string fileName;
    };

#endif
