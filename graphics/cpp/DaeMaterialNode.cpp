#include "DaeMaterialNode.h"

DaeMaterialNode::DaeMaterialNode(XMLNode *node)
: ident(), name(), effect(0)
{ 
    ident = std::string(node->getAttribute("id", 0));
    name = std::string(node->getAttribute("name", 0));
    
    int numInstanceEffectChildren = node->nChildNode("instance_effect");
    if (numInstanceEffectChildren == 1)
    {
        XMLNode xmlInstanceEffect = node->getChildNode("instance_effect", 0);
        effect = new DaeInstanceEffectNode(&xmlInstanceEffect);
    }    
}

DaeMaterialNode::~DaeMaterialNode()
{
    if (effect)
    {
        delete effect;
    }
}
