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
        MorphNode() : aspectable::Aspectable<aspectable::Aspect>(this)
        {
            if (!MorphNode::getRootNode())
                MorphNode::setRootNode(this);         
        }

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

            double l = YGNodeLayoutGetLeft(yogaNode);
            double t = YGNodeLayoutGetTop(yogaNode);
            double r = YGNodeLayoutGetRight(yogaNode);
            double b = YGNodeLayoutGetBottom(yogaNode);
            double w = YGNodeLayoutGetWidth(yogaNode);
            double h = YGNodeLayoutGetHeight(yogaNode);
            const auto [top, right, bottom, left] = getMargin();

            // 绘制背景
            auto backgroundColor = getBackgroundColor();
            SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
            SDL_FRect rect {(float)offsetX, (float)offsetY, (float)(w - l - r), (float)(h - t - b)};
            SDL_RenderFillRect(renderer, &rect);

            // 绘制边框
            auto borderColor = getBorderColor();
            SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
            SDL_RenderRect(renderer, &rect);

            // 递归渲染子节点
            for (auto& pChild : m_pChildren)
                pChild->render(renderer, offsetX, offsetY);
        }

        static MorphNode* getRootNode();
        
        static void setRootNode(MorphNode* node);
        
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
    };
}