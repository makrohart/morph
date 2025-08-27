#pragma once

#include <memory>
#include <random>
#include <set>
#include <string>

#include "../aspectable/Aspectable.h"
#include "../eventable/Eventable.h"
#include "SDL3/SDL.h"
#include "YGLayout.h"

namespace morph
{
    struct View : aspectable::Aspectable<aspectable::Aspect>
    {
        using RendererPtr = SDL_Renderer*;

        View();
        ~View();

        void setProperty(const std::string& property, const std::string& value);
        double getProperty(const std::string& property);

        virtual void addTo(View* pParentView);
        virtual void  removeFrom(View* pParentView);

        void render(RendererPtr& renderer, int offsetX, int offsetY);
        virtual void onRender(RendererPtr& renderer, int& offsetX, int& offsetY);
        inline virtual void onRendered(RendererPtr& renderer) {};

        inline ILayout* getLayout() const { return m_pLayout.get(); }
        inline int getChildrenSize() const { return m_pChildViews.size(); }

        virtual View* getSelectedNode(int x, int y);
        inline View* getParentView() { return m_pParentView; }

        void onEvent(const std::string& eventName, const std::function<void(eventable::EventArgs)>& eventHandler);
        void raiseEvent(const std::string& eventName, const eventable::EventArgs& eventArgs);

        private:
        std::unique_ptr<ILayout> m_pLayout{nullptr};
        View* m_pParentView = nullptr;
        std::set<View*> m_pChildViews;
        eventable::Eventable m_eventable;
    };
}