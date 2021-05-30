#ifndef GENG_DAE_INSTANCE_EFFECT_NODE
#define GENG_DAE_INSTANCE_EFFECT_NODE

#include "utXMLParser.h"
#include <string>
#include <cstdio>


// Reflects the "instance_effect" node
class DaeInstanceEffectNode
    {
    public:
        DaeInstanceEffectNode(XMLNode *node);
        ~DaeInstanceEffectNode();
        
        std::string getUrl() { return url; }
    private:
        std::string url;
    };

#endif
