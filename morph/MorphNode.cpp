#include "MorphNode.h"

namespace
{
    static morph::MorphNode* s_rootNode = nullptr;
}

morph::MorphNode* morph::MorphNode::getRootNode()
{
    return s_rootNode;
}

void morph::MorphNode::setRootNode(MorphNode* node)
{
    s_rootNode = node;
}