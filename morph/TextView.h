#pragma  once

#include <string>

#include "View.h"

namespace morph
{
    struct TextView : View
    {
        TextView(const std::string& text);

        ~TextView() override = default;

        void onRender(RendererPtr& renderer, int& offsetX, int& offsetY) override;
        inline void onRendered(RendererPtr& renderer) override {};

        void setText(const std::string& text);

        private:
        std::string m_text;
    };
};