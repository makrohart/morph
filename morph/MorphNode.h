#pragma once

#include <set>
#include <string>

#include "MorphNodeStyle.h"
#include "SDL3/SDL.h"
#include "yoga/Yoga.h"
#include "yoga/node/Node.h"

namespace morph
{
    struct MorphNode : virtual MorphNodeStyle
    {
        MorphNode() : aspectable::Aspectable<aspectable::Aspect>(this) {}

        ~MorphNode()
        {
            // DFS - safely delete children
            for (auto& pChild : m_pChildren)
            {
                if (pChild)
                {
                    // Remove from Yoga layout first
                    YGNodeRemoveChild(getYGNodeRef().get(), pChild->getYGNodeRef().get());
                    delete pChild;
                }
            }
            m_pChildren.clear();
        }

        static int getNextId()
        {
            static int s_id{0};
            return s_id++;
        }

        void add(MorphNode* pChild)  
        {
            if (!pChild)
                return;
            
            if (m_pChildren.find(pChild) != m_pChildren.cend())
                return;
                
            auto parentNode = getYGNodeRef();
            auto childNode = pChild->getYGNodeRef();
            
            if (!parentNode || !childNode) {
                return;
            }
            
            auto child = facebook::yoga::resolveRef(childNode.get());
            child->setOwner(nullptr);
            
            size_t index = m_pChildren.size();
            m_pChildren.insert(pChild);
            pChild->m_pParent = this;
            YGNodeInsertChild(parentNode.get(), childNode.get(), index);
        }

        void remove(MorphNode* pChild)
        {
            if (!pChild)
                return;

            if (m_pChildren.find(pChild) == m_pChildren.cend())
                return;

            // Remove from Yoga layout first
            auto parentNode = getYGNodeRef();
            auto childNode = pChild->getYGNodeRef();
            
            if (parentNode && childNode) {
                YGNodeRemoveChild(parentNode.get(), childNode.get());
            }
            
            // Remove from our set
            m_pChildren.erase(pChild);
            
            // Clear parent reference
            pChild->m_pParent = nullptr;
        }

        void render(SDL_Renderer *renderer, int offsetX, int offsetY)
        {
             YGNodeRef yogaNode = getYGNodeRef().get();

            int x = offsetX + (int)YGNodeLayoutGetLeft(yogaNode);
            int y = offsetY + (int)YGNodeLayoutGetTop(yogaNode);
            int w = (int)YGNodeLayoutGetWidth(yogaNode);
            int h = (int)YGNodeLayoutGetHeight(yogaNode);

            // 绘制背景
            MorphColor bg = getBackgroundColor();
            SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
            SDL_FRect rect {(float)x, (float)y, (float)w, (float)h};
            SDL_RenderFillRect(renderer, &rect);

            // 绘制边框
            MorphColor border = getBorderColor();
            SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
            SDL_RenderRect(renderer, &rect);

            // 递归渲染子节点
            for (auto& pChild : m_pChildren)
                pChild->render(renderer, x, y);
        }

        private:
        int m_id{MorphNode::getNextId()};
        MorphNode* m_pParent = nullptr;
        std::set<MorphNode*> m_pChildren;
    };
}