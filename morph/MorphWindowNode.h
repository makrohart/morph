#pragma once

#include "../aspectable/Aspectable.h"
#include "../journal/Journal.h"
#include "MorphNode.h"

namespace morph
{
    struct MorphWindowNode : MorphNode
    {
        MorphWindowNode() = default;
        ~MorphWindowNode() override
        {
            SDL_DestroyRenderer(m_pRenderer);
            SDL_DestroyWindow(m_pWindow);
            SDL_Quit();
        }

        void onRender(SDL_Renderer *renderer, int& offsetX, int& offsetY) override
        {
            if (renderer)
                return;
            
            const double width = getWidth();
            const double height = getHeight();

            // 2. 创建窗口
            m_pWindow = SDL_CreateWindow("morph", width, height, SDL_WINDOW_RESIZABLE);
            if (!m_pWindow)
            {
                journal::Journal<journal::Severity::Fatal>() << "SDL_CreateWindow error: " << SDL_GetError();
                SDL_Quit();
                return;
            }

            // 3. 创建渲染器
            renderer = SDL_CreateRenderer(m_pWindow, nullptr);
            m_pRenderer = renderer;
            if (!renderer) {
                journal::Journal<journal::Severity::Fatal>() << "SDL_CreateRenderer error: " << SDL_GetError();
                SDL_DestroyWindow(m_pWindow);
                SDL_Quit();
                return;
            }

            // 4. 主循环
            bool running = true;
            while (running) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_EVENT_QUIT)
                    {
                        running = false;  // 用户点击关闭按钮
                    }
                    else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        int x = event.button.x;
                        int y = event.button.y; 

                        morph::MorphNode* selectedNode = getSelectedNode(x, y);
                        if (selectedNode)
                            selectedNode->raise("onClick", eventable::EventArgs{});
                    }
                }

                // 清屏（白色背景）
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);

                // Render nodes
                YGNodeCalculateLayout(getYGNodeRef().get(), width, height, YGDirectionLTR);
                render(renderer, 0, 0);

                // 更新屏幕
                SDL_RenderPresent(renderer);
            }
        };

        private:
        SDL_Renderer* m_pRenderer;
        SDL_Window* m_pWindow;
    };
}