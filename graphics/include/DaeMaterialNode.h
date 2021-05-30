#ifndef GENG_DAE_MATERIAL_NODE
#define GENG_DAE_MATERIAL_NODE

#include "DaeInstanceEffectNode.h"
#include "utXMLParser.h"
#include <string>

// Reflects the XML node named "material"
class DaeMaterialNode
    {
    public:
        DaeMaterialNode(XMLNode *node);
        ~DaeMaterialNode();
        std::string getId() { return ident; }
        std::string getName() { return name; }
        DaeInstanceEffectNode *getEffect() { return effect; }
    private:
        std::string ident;
        std::string name;
        DaeInstanceEffectNode *effect;
    };

#endif
