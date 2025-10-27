#pragma  once

#include "../aspectable/Aspectable.h"
#include "View.h"

namespace morph
{
    struct DivView : View
    {
        DivView();
        ~DivView() override = default;

        void onRender(RendererPtr& renderer, int& offsetX, int& offsetY) override;
        inline void onRendered(RendererPtr& renderer) override {};
    };
};