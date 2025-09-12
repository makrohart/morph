#include  "WindowView.h"
#include "WindowMgr.h"
#include "SDL3_ttf/SDL_ttf.h"

namespace morph
{
    WindowView::WindowView() {}

    WindowView::~WindowView()
    {
        destroyWindow();
    }

    void WindowView::onRender(RendererPtr& renderer, int& offsetX, int& offsetY)
    {
        m_pLocalRenderer = renderer;
        renderer = m_pRenderer;
        const double width = getProperty("styleWidth");
        const double height = getProperty("styleHeight");
        getLayout()->calculate(width, height);

        // 清屏（白色背景）
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
    };

    void WindowView::onRendered(RendererPtr& renderer)
    {
        // 更新屏幕
        SDL_RenderPresent(renderer);
        renderer = m_pLocalRenderer;
    }

    void WindowView::addTo(View* pParentView)
    {              
        if (!pParentView)
            return;

        // WindowView will not be added to a View other than WindowView.
        // A View tree always have a WindowView as root.
        // Instead, add this WindowView to WindowMgr
        WindowMgr& windowMgr = WindowMgr::getInstance();
        windowMgr.addWindow(this);
        
        journal::Journal<journal::Severity::Info>() << "WindowView added to WindowMgr instead of View tree";
    }

    void WindowView::removeFrom(View* pParentView)
    {
        if (!pParentView)
            return;

        // Remove WindowView from WindowMgr instead of View tree
        WindowMgr& windowMgr = WindowMgr::getInstance();
        windowMgr.removeWindow(this);
        
        journal::Journal<journal::Severity::Info>() << "WindowView removed from WindowMgr";
    }

    void WindowView::createWindow()
    {
        const double width = getProperty("styleWidth");
        const double height = getProperty("styleHeight");

        // Create window
        m_pWindow = SDL_CreateWindow("morph", static_cast<int>(width), static_cast<int>(height), SDL_WINDOW_RESIZABLE);
        if (!m_pWindow)
        {
            journal::Journal<journal::Severity::Fatal>() << "SDL_CreateWindow error: " << SDL_GetError();
            return;
        }

        // Get window ID
        m_windowId = SDL_GetWindowID(m_pWindow);

        // Create renderer
        SDL_Renderer* renderer = SDL_CreateRenderer(m_pWindow, nullptr);
        m_pRenderer = renderer;
        if (!renderer) {
            journal::Journal<journal::Severity::Fatal>() << "SDL_CreateRenderer error: " << SDL_GetError();
            SDL_DestroyWindow(m_pWindow);
            m_pWindow = nullptr;
            return;
        }
    }

    void WindowView::destroyWindow()
    {
        if (m_pRenderer)
        {
            SDL_DestroyRenderer(m_pRenderer);
            m_pRenderer = nullptr;
        }
        
        if (m_pWindow)
        {
            SDL_DestroyWindow(m_pWindow);
            m_pWindow = nullptr;
        }
        
        m_windowId = 0;
    }

    void WindowView::render()
    {
        if (!m_pRenderer)
            return;
            
        RendererPtr pRenderer = nullptr;
        View::render(pRenderer, 0, 0);
    }
}