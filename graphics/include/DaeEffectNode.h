#ifndef GENG_DAE_EFFECT_NODE
#define GENG_DAE_EFFECT_NODE

#include "utXMLParser.h"
#include <string>
#include <vector>
#include <cstdio>


// Reflects the "effect" node
class DaeEffectNode
    {
    public:
        DaeEffectNode(XMLNode *node);
        ~DaeEffectNode();
        
        std::string getId() { return ident; }
        std::string getName() { return name; }
        std::string getImageId() { return imageId; }
    private:
        std::string ident;
        std::string name;
        std::string imageId;
    };

#endif
