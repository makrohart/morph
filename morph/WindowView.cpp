#include  "WindowView.h"

#include "SDL3_ttf/SDL_ttf.h"

namespace
{
    static morph::WindowView* s_pRootWindowView = nullptr;
}

namespace morph
{
    WindowView::WindowView() {}

    WindowView::~WindowView()
    {
        SDL_DestroyRenderer(m_pRenderer);
        SDL_DestroyWindow(m_pWindow);
        SDL_Quit();
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

        if (WindowView* pWindowView = WindowView::getRootWindowView(); pWindowView != this)
        {
            m_pChildWindowView = pWindowView;
            WindowView::setRootWindowView(this);
        }
    }

    void WindowView::removeFrom(View* pParentView)
    {
        if (!pParentView)
            return;

        WindowView* pWindowView= WindowView::getRootWindowView();
        while (pWindowView->m_pChildWindowView)
        {
            if (pWindowView->m_pChildWindowView == this)
            {
                auto& nextWindowView = pWindowView->m_pChildWindowView->m_pChildWindowView;
                pWindowView->m_pChildWindowView = nullptr;
                pWindowView->m_pChildWindowView = nextWindowView;
            }
        }
    }

    WindowView* WindowView::findWindowById(const SDL_WindowID id)
    {
        const SDL_WindowID windowId = SDL_GetWindowID(m_pWindow);
        if (windowId == id)
            return this;

        return m_pChildWindowView->findWindowById(id);
    }

    void WindowView::show()
    {
        const bool bRootWindow = WindowView::getRootWindowView() == this;
        if (bRootWindow)
        {
            // 1. 初始化 SDL2 窗口信息
            if (!SDL_Init(SDL_INIT_VIDEO))
            {
                journal::Journal<journal::Severity::Fatal>() << "SDL_Init error: " << SDL_GetError();
                return;
            }

            if (!TTF_Init())
            {
                journal::Journal<journal::Severity::Fatal>() << "TTF_Init error: " << SDL_GetError();
                return;
            }         
        }

        const double width = getProperty("styleWidth");
        const double height = getProperty("styleHeight");

        // 2. 创建窗口
        m_pWindow = SDL_CreateWindow("morph", width, height, SDL_WINDOW_RESIZABLE);
        if (!m_pWindow)
        {
            journal::Journal<journal::Severity::Fatal>() << "SDL_CreateWindow error: " << SDL_GetError();
            SDL_Quit();
            return;
        }

        // 3. 创建渲染器
        SDL_Renderer* renderer = SDL_CreateRenderer(m_pWindow, nullptr);
        m_pRenderer = renderer;
        if (!renderer) {
            journal::Journal<journal::Severity::Fatal>() << "SDL_CreateRenderer error: " << SDL_GetError();
            SDL_DestroyWindow(m_pWindow);
            SDL_Quit();
            return;
        }

        // 递归渲染子节点
        if (m_pChildWindowView)
            m_pChildWindowView->show();

        // Do not enter into loop if it is not top window
        if (!bRootWindow)
            return;

        // 4. 主循环
        bool running = true;
        while (running) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;  // 用户点击关闭按钮
                }
                else if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED)
                {
                    // 任意窗口的关闭请求都退出主循环
                    running = false;
                }
                else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                {
                    WindowView* pWindowView = WindowView::getRootWindowView();
                    WindowView* pFoundWindowView = pWindowView ? pWindowView->findWindowById(event.button.windowID) : nullptr;
                    if (pFoundWindowView)
                    {
                        int x = event.button.x;
                        int y = event.button.y; 

                        morph::View* selectedView = pFoundWindowView->getSelectedNode(x, y);
                        if (selectedView)
                            selectedView->raiseEvent("onClick", eventable::EventArgs{});
                    }         
                }
            }
            RendererPtr pRenderer = nullptr;
            WindowView* pWindowView = WindowView::getRootWindowView();
            while (pWindowView)
            {
                pWindowView->render(pRenderer, 0, 0);
                pWindowView = pWindowView->m_pChildWindowView;
            }
        }
    }

    WindowView* WindowView::getRootWindowView()
    {
        return s_pRootWindowView;
    } 

    void WindowView::setRootWindowView(WindowView* pWindowView)
    {
        s_pRootWindowView = pWindowView;
    }

}