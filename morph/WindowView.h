#pragma once

#include "../aspectable/Aspectable.h"
#include "../journal/Journal.h"
#include "View.h"

namespace morph
{
    struct WindowView : View
    {
        WindowView();
        ~WindowView() override;

        void onRender(RendererPtr& renderer, int& offsetX, int& offsetY) override;
        void onRendered(RendererPtr& renderer) override;
        
        void addTo(View* pParentView) override;
        void removeFrom(View* pParentView) override;

        void createWindow();
        void destroyWindow();
        void render();
        SDL_WindowID getWindowId() const { return m_windowId; }

        private:
        // m_pLocalRenderer is a temp storage for parent renderer
        SDL_Renderer* m_pLocalRenderer = nullptr;
        SDL_Renderer* m_pRenderer = nullptr;
        SDL_Window* m_pWindow = nullptr;
        SDL_WindowID m_windowId = 0;
    };
}