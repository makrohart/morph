#pragma once

#include "../eventable/EventArgs.h"
#include "View.h"

namespace morph
{
    struct YGLayoutChangedEventArgs : eventable::EventArgs
    {
        YGLayoutChangedEventArgs(View* pParentView, const bool isAdded) : ParentView(pParentView), IsAdded(isAdded) {}
        const View* ParentView;   
        const bool IsAdded;
    };
}