#include "MorphNode.h"

namespace
{
    static morph::MorphNode* s_rootNode = nullptr;
}

namespace morph
{
    MorphNode::MorphNode()
    {
        if (!MorphNode::getRootNode())
            MorphNode::setRootNode(this);
            
        setBackgroundColorR(240);
        setBackgroundColorG(240);
        setBackgroundColorB(240);
        setBackgroundColorA(255);

        setBorderColorR(255);
        setBorderColorG(255);
        setBorderColorB(255);
        setBorderColorA(255);

        setHeightPercent(100);
    }

    MorphNode::~MorphNode()
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

    void MorphNode::add(MorphNode* pChild)  
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

    void MorphNode::remove(MorphNode* pChild)
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

    void MorphNode::onRender(SDL_Renderer *renderer, int& offsetX, int& offsetY)
    {
        YGNodeRef yogaNode = getYGNodeRef().get();

        double l = YGNodeLayoutGetLeft(yogaNode);
        double t = YGNodeLayoutGetTop(yogaNode);
        double r = YGNodeLayoutGetRight(yogaNode);
        double b = YGNodeLayoutGetBottom(yogaNode);
        double w = YGNodeLayoutGetWidth(yogaNode);
        double h = YGNodeLayoutGetHeight(yogaNode);

        offsetX += l;
        offsetY += t;

        // 绘制背景
        auto backgroundColor = getBackgroundColor();
        SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        SDL_FRect rect {(float)offsetX, (float)offsetY, (float)w, (float)h};
        SDL_RenderFillRect(renderer, &rect);

        // // 绘制边框
        auto borderColor = getBorderColor();
        SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
        SDL_RenderRect(renderer, &rect);
    }

    void MorphNode::render(SDL_Renderer *renderer, int offsetX, int offsetY)
    {
        onRender(renderer, offsetX, offsetY);

        // 递归渲染子节点
        for (auto& pChild : m_pChildren)
            pChild->render(renderer, offsetX, offsetY);
    }

    MorphNode* MorphNode::getSelectedNode(int x, int y)
    {
        YGNodeRef yogaNode = getYGNodeRef().get();

        double width = YGNodeLayoutGetWidth(yogaNode);
        double height = YGNodeLayoutGetHeight(yogaNode);
        double top = YGNodeLayoutGetTop(yogaNode);
        double left = YGNodeLayoutGetLeft(yogaNode);       
        double button = top + height;
        double right = width + width;

        if (x < left || x > right || y < top || y > button)
            return nullptr;

        x -= left;
        y -= top;

        for (auto& pChild : m_pChildren)
        {
            MorphNode* pFind = pChild->getSelectedNode(x, y);
            if (pFind)
                return pFind;
        }

        return this;
    }

    MorphNode* morph::MorphNode::getRootNode()
    {
        return s_rootNode;
    }

    void MorphNode::setRootNode(MorphNode* node)
    {
        s_rootNode = node;
    }
}
