#pragma once

#include <functional>
#include <vector>

#include "EventArgs.h"

namespace eventable
{
    struct Event
    {
        virtual ~Event() = default;

        void on(const std::function<void(EventArgs)>& eventHandler)
        {
            m_eventHandlers.push_back(eventHandler);
        }

        void raise(const EventArgs& eventArgs)
        {
            for (auto& eventHandler : m_eventHandlers)
                eventHandler(eventArgs);
        }

        private:
        std::vector<std::function<void(EventArgs)>> m_eventHandlers;
    };
}