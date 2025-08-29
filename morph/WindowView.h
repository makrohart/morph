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

        void show();

        WindowView* findWindowById(const SDL_WindowID id);

        static WindowView* getRootWindowView();    
        static void setRootWindowView(WindowView* pWindowView);

        private:
        // m_pLocalRenderer is a temp storage for parent renderer
        SDL_Renderer* m_pLocalRenderer;
        SDL_Renderer* m_pRenderer;
        SDL_Window* m_pWindow;

        WindowView* m_pChildWindowView = nullptr;
    };
}