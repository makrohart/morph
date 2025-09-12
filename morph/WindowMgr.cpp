#include "WindowMgr.h"
#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "../journal/Journal.h"
#include <algorithm>

namespace morph
{
    WindowMgr& WindowMgr::getInstance()
    {
        static WindowMgr instance;
        return instance;
    }
    
    void WindowMgr::addWindow(WindowView* window)
    {
        if (window && std::find(m_windows.begin(), m_windows.end(), window) == m_windows.end())
        {
            m_windows.push_back(window);
        }
    }
    
    void WindowMgr::removeWindow(WindowView* window)
    {
        auto it = std::find(m_windows.begin(), m_windows.end(), window);
        if (it != m_windows.end())
        {
            m_windows.erase(it);
        }
    }
    
    void WindowMgr::clearAllWindows()
    {
        m_windows.clear();
        journal::Journal<journal::Severity::Info>() << "All windows cleared from manager";
    }
    
    void WindowMgr::run()
    {
        if (m_windows.empty())
        {
            journal::Journal<journal::Severity::Fatal>() << "No windows to run!";
            return;
        }
        
        // Initialize SDL and TTF only once
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            journal::Journal<journal::Severity::Fatal>() << "SDL_Init error: " << SDL_GetError();
            return;
        }
        
        if (TTF_Init() < 0)
        {
            journal::Journal<journal::Severity::Fatal>() << "TTF_Init error: " << SDL_GetError();
            SDL_Quit();
            return;
        }
        
        // Create all windows
        for (auto window : m_windows)
        {
            if (window)
            {
                window->createWindow();
            }
        }
        
        m_running = true;
        
        // Main event loop
        while (m_running)
        {
            handleEvents();
            renderAll();
        }
        
        // Cleanup
        TTF_Quit();
        SDL_Quit();
    }
    
    void WindowMgr::renderAll()
    {
        for (auto window : m_windows)
        {
            if (window)
            {
                window->render();
            }
        }
    }
    
    void WindowMgr::handleEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    m_running = false;
                    break;
                    
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    // Find and remove the window that requested to close
                    {
                        WindowView* windowToClose = findWindowById(event.window.windowID);
                        if (windowToClose)
                        {
                            removeWindow(windowToClose);
                            windowToClose->destroyWindow();
                            
                            // If no windows left, exit
                            if (m_windows.empty())
                            {
                                m_running = false;
                            }
                        }
                    }
                    break;
                    
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    {
                        WindowView* targetWindow = findWindowById(event.button.windowID);
                        if (targetWindow)
                        {
                            int x = event.button.x;
                            int y = event.button.y;
                            
                            morph::View* selectedView = targetWindow->getSelectedNode(x, y);
                            if (selectedView)
                            {
                                selectedView->raiseEvent("onClick", eventable::EventArgs{});
                            }
                        }
                    }
                    break;
                    
                default:
                    break;
            }
        }
    }
    
    WindowView* WindowMgr::findWindowById(SDL_WindowID id)
    {
        for (auto window : m_windows)
        {
            if (window && window->getWindowId() == id)
            {
                return window;
            }
        }
        return nullptr;
    }
}
