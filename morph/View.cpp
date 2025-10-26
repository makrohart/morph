#include "View.h"
#include "YGLayout.h"
#include "YGLayoutChangedEventArgs.h"

namespace morph
{

    View::View() : aspectable::Aspectable<aspectable::Aspect>(this)
    {
        setProperty("backgroundColorR", "240");
        setProperty("backgroundColorG", "240");
        setProperty("backgroundColorB", "240");
        setProperty("backgroundColorA", "255");

        setProperty("borderColorR", "255");
        setProperty("borderColorG", "255");
        setProperty("borderColorB", "255");
        setProperty("borderColorA", "255");

        setProperty("heightPercent", "100");

        m_pLayout = std::make_unique<YGLayout>();
    }

    View::~View()
    {
        // DFS - safely delete children
        for (auto pChildView : m_pChildViews)
        {
            if (pChildView)
            {
                // Remove from Yoga layout first
                m_pLayout->onLayoutChanged(YGLayoutChangedEventArgs{this, /*IsAdded=*/false});
                delete pChildView;
            }
        }
        m_pChildViews.clear();
    }

    void View::setProperty(const std::string& property, const std::string& value)
    {
        if (m_pLayout)
            m_pLayout->setProperty(property, value);
    }

    double View::getProperty(const std::string& property)
    {
        if (m_pLayout)
            return m_pLayout->getProperty(property);

        throw std::runtime_error("Layout not initialized or property not supported: " + property);
    }

    std::array<double, 4> View::getBoundingBox()
    {
        const double top = getProperty("top");
        const double left = getProperty("left");
        const double width = getProperty("width");
        const double height = getProperty("height");

        return {top, left, width, height};
    }

    void View::addTo(View* pParentView)  
    {
        if (!pParentView)
            return;
        
        if (pParentView->m_pChildViews.find(this) != pParentView->m_pChildViews.cend())
            return;
        
        m_pParentView = pParentView;
        m_pLayout->onLayoutChanged(YGLayoutChangedEventArgs{pParentView, /*IsAdded=*/true});
        // Insertion should after notifying YGLayout
        pParentView->m_pChildViews.insert(this);
    }

    void View::removeFrom(View* pParentView)
    {
        if (!pParentView)
            return;

        if (pParentView->m_pChildViews.find(this) == pParentView->m_pChildViews.cend())
            return;

        // Remove from Yoga layout first
        m_pLayout->onLayoutChanged(YGLayoutChangedEventArgs{pParentView, /*IsAdded=*/false});
        
        // Remove from our set
        pParentView->m_pChildViews.erase(this);
        
        // Clear parent reference
        m_pParentView = nullptr;
    }

    void View::onRender(RendererPtr& renderer, int& offsetX, int& offsetY)
    {
    }

    void View::render(RendererPtr& renderer, int offsetX, int offsetY)
    {
        onRender(renderer, offsetX, offsetY);

        // 递归渲染子节点
        for (auto& pChild : m_pChildViews)
            pChild->render(renderer, offsetX, offsetY);

        onRendered(renderer);
    }

    View* View::getSelectedNode(int x, int y)
    {
        double top = getProperty("top");
        double left = getProperty("left");
        double width = getProperty("width");
        double height = getProperty("height");

        double bottom = top + height;
        double right = left + width;

        if (x < left || x > right || y < top || y > bottom)
            return nullptr;

        x -= left;
        y -= top;

        for (auto& pChild : m_pChildViews)
        {
            View* pFind = pChild->getSelectedNode(x, y);
            if (pFind)
                return pFind;
        }

        return this;
    }

    void View::onEvent(const std::string& eventName, const std::function<void(eventable::EventArgs)>& eventHandler)
    {
        m_eventable.onEvent(eventName, eventHandler);
    }

    void View::removeEvent(const std::string& eventName, const std::function<void(eventable::EventArgs)>& eventHandler)
    {
        m_eventable.removeEvent(eventName, eventHandler);
    }

    void View::raiseEvent(const std::string& eventName, const eventable::EventArgs& eventArgs)
    {
        const bool bRaiseEvent = canRaiseEvent(eventArgs.position.at(0), eventArgs.position.at(1));
        if (bRaiseEvent)
            m_eventable.raiseEvent(eventName, eventArgs);

        // Recursively raise event down along the tree
        for (auto& pChild : m_pChildViews)
            pChild->raiseEvent(eventName, eventArgs);
    }

    bool View::canRaiseEvent(double x, double y)
    {
        const auto [top, left, width, height] = getBoundingBox();
        return x >= left && 
               x <= (left + width) &&
               y >= top && 
               y <= (top + height);
    }
    
    // Rendering helper methods implementation
    void View::renderBackground(SDL_Renderer* renderer, const ILayout::Color& backgroundColor, 
                              float x, float y, float width, float height)
    {
        SDL_SetRenderDrawColor(renderer, backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
        SDL_FRect rect{x, y, width, height};
        SDL_RenderFillRect(renderer, &rect);
    }
    
    void View::renderBorder(SDL_Renderer* renderer, const ILayout::Color& borderColor, 
                          float x, float y, float width, float height)
    {
        SDL_SetRenderDrawColor(renderer, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
        SDL_FRect rect{x, y, width, height};
        SDL_RenderRect(renderer, &rect);
    }
    
    void View::renderBackgroundAndBorder(SDL_Renderer* renderer, 
                                       const ILayout::Color& backgroundColor,
                                       const ILayout::Color& borderColor,
                                       float x, float y, float width, float height)
    {
        renderBackground(renderer, backgroundColor, x, y, width, height);
        renderBorder(renderer, borderColor, x, y, width, height);
    }
    
    ILayout::Color View::getColorFromProperties(double r, double g, double b, double a)
    {
        return {static_cast<unsigned char>(r), static_cast<unsigned char>(g), 
                static_cast<unsigned char>(b), static_cast<unsigned char>(a)};
    }
}
