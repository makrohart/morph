#include <algorithm>
#include <iostream>

#include "aspectable/Aspectable.h"
#include "journal/Journal.h"
#include "jsEngine/JSEngine.h"
#include "jsEngine/V8Binding.h"
#include "jsEngine/V8Converter.h"
#include "jsEngine/V8Engine.h"
#include "needle/Sewable.h"
#include "morph/mvvm/ViewModel.h"
#include "morph/MorphNode.h"

struct A : public aspectable::Aspectable<aspectable::Aspect>
{
    A() : aspectable::Aspectable<aspectable::Aspect>(this) {}

    int getIntField() { return m_intField; }
    void setIntField(const int value) { m_intField = value; }

    int run(int) 
    { 
        journal::Journal<journal::Severity::Info>() << "A::run";
        return 1; 
    }

    int m_intField;
};

int main(int argc, const char* argv[])
{
    needle::Sewable<"A", nullptr, A>().accept<V8Binding>();
    needle::Sewable<"run", &A::run, decltype(&A::run)>().accept<V8Binding>();
    needle::Sewable<"m_intField", &A::m_intField, decltype(&A::m_intField)>().accept<V8Binding>();
    needle::Sewable<"IntField", std::pair(&A::getIntField, &A::setIntField), decltype(&A::m_intField)>().accept<V8Binding>();

    needle::Sewable<"Journal", nullptr, journal::Journal<journal::Severity::Info>>().accept<V8Binding>();
    needle::Sewable<"log", &journal::Journal<journal::Severity::Info>::log, decltype(&journal::Journal<journal::Severity::Info>::log)>().accept<V8Binding>();

    needle::Sewable<"ViewModel", nullptr, mvvm::ViewModel>().accept<V8Binding>();
    needle::Sewable<"onPropertyChanged", &mvvm::ViewModel::onPropertyChanged, decltype(&mvvm::ViewModel::onPropertyChanged)>().accept<V8Binding>();
    needle::Sewable<"notifyPropertyChanged", &mvvm::ViewModel::notifyPropertyChanged, decltype(&mvvm::ViewModel::notifyPropertyChanged)>().accept<V8Binding>();
    needle::Sewable<"IntField", std::pair(&mvvm::ViewModel::getIntField, &mvvm::ViewModel::setIntField), decltype(&mvvm::ViewModel::m_intField)>().accept<V8Binding>();
    needle::Sewable<"StringField", std::pair(&mvvm::ViewModel::getStringField, &mvvm::ViewModel::setStringField), decltype(&mvvm::ViewModel::m_stringField)>().accept<V8Binding>();
    needle::Sewable<"addInt", &mvvm::ViewModel::addInt, decltype(&mvvm::ViewModel::addInt)>().accept<V8Binding>();

    needle::Sewable<"MorphNode", nullptr, morph::MorphNode>().accept<V8Binding>();
    needle::Sewable<"add", &morph::MorphNode::add, decltype(&morph::MorphNode::add)>().accept<V8Binding>();
    needle::Sewable<"remove", &morph::MorphNode::remove, decltype(&morph::MorphNode::remove)>().accept<V8Binding>();

    journal::Journal<journal::Severity::Info>()<< "Hello " << "able";
    journal::Journal<journal::Severity::Fatal>()<< "Hello " << "able";

    JSEngine* engine = new V8Engine();
    engine->run({
        "D:/Projects/able/reacts/dist/reacts.umd.js",
        "D:/Projects/able/out/build/x64-debug/Debug/morph.js",
        "D:/Projects/able/out/build/x64-debug/Debug/journal.js",
    });
    std::cout << "Hello, able!\n";

    // 初始化 SDL2 窗口信息
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        journal::Journal<journal::Severity::Fatal>() << "SDL_Init error: " << SDL_GetError();
        return -1;
    }

    // 2. 创建窗口
    SDL_Window* window = SDL_CreateWindow("able", 1000, 600, SDL_WINDOW_RESIZABLE);

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
            if (event.type == SDL_EVENT_QUIT) {
                running = false;  // 用户点击关闭按钮
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
