#pragma once

#include <list>
#include <memory>
#include "WindowView.h"

namespace morph
{
    class WindowMgr
    {
    public:
        // Singleton access
        static WindowMgr& getInstance();
        
        // Window management
        void addWindow(WindowView* window);
        void removeWindow(WindowView* window);
        void clearAllWindows();
        
        // Main loop and rendering
        void run();
        void renderAll();
        void handleEvents();
        
        // Getters
        size_t getWindowCount() const { return m_windows.size(); }
        bool isEmpty() const { return m_windows.empty(); }
        
        // Find window by ID
        WindowView* findWindowById(SDL_WindowID id);
        
    private:
        WindowMgr() = default;
        ~WindowMgr() = default;
        
        // Disable copy constructor and assignment operator
        WindowMgr(const WindowMgr&) = delete;
        WindowMgr& operator=(const WindowMgr&) = delete;
        
        std::list<WindowView*> m_windows;
        bool m_running = false;
    };
}
