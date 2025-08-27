#pragma once

#include <map>
#include <string>

#include "Event.h"
#include "EventArgs.h"

namespace eventable
{
    struct Eventable
    {

        void onEvent(const std::string& event, const std::function<void(EventArgs)>& eventHandler)
        {
            if (m_events.find(event) == m_events.cend())
                m_events[event] = Event{};

            m_events[event].on(eventHandler);       
        }

        void raiseEvent(const std::string& event, const EventArgs& eventArgs)
        {
            if (m_events.find(event) == m_events.cend())
                return;

            m_events[event].raise(eventArgs);
        }

        private:
        std::map<std::string, Event> m_events;
    };
}