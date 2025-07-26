#pragma once

#include <random>
#include <set>
#include <string>

#include "../eventable/Eventable.h"
#include "MorphNodeStyle.h"
#include "SDL3/SDL.h"
#include "yoga/Yoga.h"
#include "yoga/node/Node.h"

namespace morph
{
    struct MorphNode : MorphNodeStyle
    {
        MorphNode();

        ~MorphNode() override;

        void add(MorphNode* pChild);

        void remove(MorphNode* pChild);

        void render(SDL_Renderer *renderer, int offsetX, int offsetY);
        virtual void onRender(SDL_Renderer *renderer, int& offsetX, int& offsetY);

        virtual MorphNode* getSelectedNode(int x, int y);

        static MorphNode* getRootNode();    
        static void setRootNode(MorphNode* node);
        
        inline void on(const std::string& eventName, const std::function<void(eventable::EventArgs)>& eventHandler)
        {
            m_eventable.on(eventName, eventHandler);
        }

        inline void raise(const std::string& eventName, const eventable::EventArgs& eventArgs)
        {
            m_eventable.raise(eventName, eventArgs);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, 255);
            int r = dis(gen); // 红色分量 (0-255)
            int g = dis(gen); // 绿色分量 (0-255)
            int b = dis(gen); // 蓝色分量 (0-255)
            setBackgroundColorR(r);
            setBackgroundColorG(g);
            setBackgroundColorB(b);
        }

        private:
        static int getNextId()
        {
            static int s_id{0};
            return s_id++;
        }

        private:
        int m_id{MorphNode::getNextId()};
        MorphNode* m_pParent = nullptr;
        std::set<MorphNode*> m_pChildren;
        eventable::Eventable m_eventable;
    };
}