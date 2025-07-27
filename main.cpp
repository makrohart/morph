#include <algorithm>
#include <iostream>

#include "aspectable/Aspectable.h"
#include "journal/Journal.h"
#include "bridge/JSEngine.h"
#include "bridge/v8/V8Bridge.h"
#include "bridge/v8/V8Engine.h"
#include "needle/Sewable.h"
#include "needle/FixedString.h"
#include "add-ins/ViewModel.h"
#include "morph/MorphButtonNode.h"
#include "morph/MorphNode.h"
#include "morph/MorphTimer.h"

struct A : public aspectable::Aspectable<aspectable::Aspect>
{
    A() : aspectable::Aspectable<aspectable::Aspect>(this) {}

    int getIntField() { return m_intField; }
    void setIntField(const int value) { m_intField = value; }

    int run(const int) 
    { 
        journal::Journal<journal::Severity::Info>() << "A::run";
        return 1; 
    }

    int m_intField;
};

int main(int argc, const char* argv[])
{
    needle::Sewable<"A", nullptr, A>().accept<V8Bridge>();
    needle::Sewable<"m_intField", &A::m_intField, decltype(&A::m_intField)>().accept<V8Bridge>();
    needle::Sewable<"run", &A::run, decltype(&A::run)>().accept<V8Bridge>();
    needle::Sewable<"IntField", &A::getIntField, decltype(&A::getIntField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"IntField", &A::setIntField, decltype(&A::setIntField), needle::Property>().accept<V8Bridge>();

    needle::Sewable<"Journal", nullptr, journal::Journal<journal::Severity::Info>>().accept<V8Bridge>();
    // TODO: aspectable not support const for referenced arguement
    needle::Sewable<"log", &journal::Journal<journal::Severity::Info>::log, decltype(&journal::Journal<journal::Severity::Info>::log)>().accept<V8Bridge>();

    needle::Sewable<"ViewModel", nullptr, mvvm::ViewModel>().accept<V8Bridge>();
    needle::Sewable<"onPropertyChanged", &mvvm::ViewModel::onPropertyChanged, decltype(&mvvm::ViewModel::onPropertyChanged)>().accept<V8Bridge>();
    needle::Sewable<"notifyPropertyChanged", &mvvm::ViewModel::notifyPropertyChanged, decltype(&mvvm::ViewModel::notifyPropertyChanged)>().accept<V8Bridge>();
    needle::Sewable<"IntField", &mvvm::ViewModel::getIntField, decltype(&mvvm::ViewModel::getIntField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"IntField", &mvvm::ViewModel::setIntField, decltype(&mvvm::ViewModel::setIntField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"StringField", &mvvm::ViewModel::getStringField, decltype(&mvvm::ViewModel::getStringField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"StringField", &mvvm::ViewModel::setStringField, decltype(&mvvm::ViewModel::setStringField), needle::Property>().accept<V8Bridge>();
    needle::Sewable<"addInt", &mvvm::ViewModel::addInt, decltype(&mvvm::ViewModel::addInt)>().accept<V8Bridge>();

    needle::Sewable<"MorphNode", nullptr, morph::MorphNode>().accept<V8Bridge>();
    needle::Sewable<"add", &morph::MorphNode::add, decltype(&morph::MorphNode::add)>().accept<V8Bridge>();
    needle::Sewable<"remove", &morph::MorphNode::remove, decltype(&morph::MorphNode::remove)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::MorphNode::setProperty, decltype(&morph::MorphNode::setProperty)>().accept<V8Bridge>();

    needle::Sewable<"MorphButtonNode", nullptr, morph::MorphButtonNode>().accept<V8Bridge>();
    needle::Sewable<"add", &morph::MorphButtonNode::add, decltype(&morph::MorphButtonNode::add)>().accept<V8Bridge>();
    needle::Sewable<"remove", &morph::MorphButtonNode::remove, decltype(&morph::MorphButtonNode::remove)>().accept<V8Bridge>();
    needle::Sewable<"setProperty", &morph::MorphButtonNode::setProperty, decltype(&morph::MorphButtonNode::setProperty)>().accept<V8Bridge>();
    needle::Sewable<"on", &morph::MorphButtonNode::on, decltype(&morph::MorphButtonNode::on)>().accept<V8Bridge>();


    needle::Sewable<"MorphTimer", nullptr, morph::MorphTimer>().accept<V8Bridge>();
    needle::Sewable<"setTimeout", &morph::MorphTimer::setTimeout, decltype(&morph::MorphTimer::setTimeout)>().accept<V8Bridge>();
    needle::Sewable<"clearTimeout", &morph::MorphTimer::clearTimeout, decltype(&morph::MorphTimer::clearTimeout)>().accept<V8Bridge>();

    journal::Journal<journal::Severity::Info>()<< "Hello " << "morph";
    journal::Journal<journal::Severity::Fatal>()<< "Hello " << "morph";

    JSEngine* engine = new V8Engine();
    // Order matters
    engine->run({
        "D:/Projects/morph/out/build/x64-debug/Debug/morph-api.js",
        "D:/Projects/morph/reacts/dist/reacts.umd.js",
        "D:/Projects/morph/out/build/x64-debug/Debug/morph.js",
        "D:/Projects/morph/out/build/x64-debug/Debug/journal.js",
    });  

    std::cout << "Hello, morph!\n";

    // 初始化 SDL2 窗口信息
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        journal::Journal<journal::Severity::Fatal>() << "SDL_Init error: " << SDL_GetError();
        return -1;
    }

    // 2. 创建窗口
    SDL_Window* window = SDL_CreateWindow("morph", 1000, 600, SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        journal::Journal<journal::Severity::Fatal>() << "SDL_CreateWindow error: " << SDL_GetError();
        SDL_Quit();
        return -1;
    }

    // 3. 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer) {
        journal::Journal<journal::Severity::Fatal>() << "SDL_CreateRenderer error: " << SDL_GetError();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
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

                morph::MorphNode* pRootNode = morph::MorphNode::getRootNode();
                if (pRootNode)
                {
                    morph::MorphNode* selectedNode =  pRootNode->getSelectedNode(x, y);
                    selectedNode->raise("onClick", eventable::EventArgs{});
                }
            }
        }

        // 清屏（白色背景）
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Render nodes
        morph::MorphNode* pRootNode = morph::MorphNode::getRootNode();
        if (pRootNode)
        {
            YGNodeCalculateLayout(pRootNode->getYGNodeRef().get(), 1000, 600, YGDirectionLTR);
            pRootNode->render(renderer, 0, 0);
        }

        // 更新屏幕
        SDL_RenderPresent(renderer);
    }

    // 5. 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
