#pragma once

#include "yoga/YGNodeStyle.h"

namespace morph
{
    struct NodeStyle
    {
        double flex;
        double margin;
        YGFlexDirection flexDirection;
        YGJustify justifyContent;
    }
}