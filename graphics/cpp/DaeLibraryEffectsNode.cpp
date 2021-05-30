#include "DaeLibraryEffectsNode.h"

DaeLibraryEffectsNode::DaeLibraryEffectsNode(XMLNode *node)
: effects()
{
    // process the "effect" elements
    int numEffectElements = node->nChildNode("effect");
    if (numEffectElements <= 0)
    {
        printf("No effect elements found in library_effects element.\n");
        return;
    }
    for (int i = 0; i < numEffectElements; i++)
    {
        XMLNode child = node->getChildNode(i);
        effects.push_back(new DaeEffectNode(&child));
    }
}

DaeLibraryEffectsNode::~DaeLibraryEffectsNode()
{
    effects.clear();
}