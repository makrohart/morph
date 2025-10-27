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
            m_eventHandler = eventHandler;
        }

        void raise(const EventArgs& eventArgs)
        {
            m_eventHandler(eventArgs);
        }

        private:
        std::function<void(EventArgs)> m_eventHandler;
    };
}