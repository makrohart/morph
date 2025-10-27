#pragma  once

#include "View.h"

namespace morph
{
    struct ButtonView : View
    {
        ButtonView();

        ~ButtonView() override = default;

        void onRender(RendererPtr& renderer, int& offsetX, int& offsetY) override;
        inline void onRendered(RendererPtr& renderer) override {};
    };
};