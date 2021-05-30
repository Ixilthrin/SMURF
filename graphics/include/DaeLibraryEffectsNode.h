#ifndef GENG_DAE_LIBRARY_EFFECTS_NODE
#define GENG_DAE_LIBRARY_EFFECTS_NODE

#include "DaeEffectNode.h"
#include "utXMLParser.h"
#include <vector>

// Reflects the "library_effects" node
class DaeLibraryEffectsNode
    {
    public:
        DaeLibraryEffectsNode(XMLNode *node);
        ~DaeLibraryEffectsNode();
        std::vector<DaeEffectNode *> getEffects() { return effects; }
    private:
        std::vector<DaeEffectNode *> effects;
    };

#endif
