#pragma once

#include <functional>
#include <string>
#include <list>
#include <unordered_map>

#include "aspectable/Aspectable.h"

namespace mvvm
{
    struct ViewModel : public aspectable::Aspectable<aspectable::Aspect>
    {
        using PropertyChangedEventHandler = std::function<void()>;

        ViewModel() : aspectable::Aspectable<aspectable::Aspect>(this) {}

        virtual ~ViewModel() = default;

        inline void onPropertyChanged(const std::string& propertyName, const PropertyChangedEventHandler& eventHandler)
        {
            if (m_eventHandlers.find(propertyName) == m_eventHandlers.cend())
                m_eventHandlers[propertyName] = std::move(std::list<PropertyChangedEventHandler>());

            m_eventHandlers[propertyName].push_back(eventHandler);
        }

        inline void notifyPropertyChanged(const std::string& propertyName)
        {
            if (m_eventHandlers.find(propertyName) == m_eventHandlers.cend())
                return;

            for (PropertyChangedEventHandler& eventHandler: m_eventHandlers[propertyName])
            {
                eventHandler();
            }
        }

        void addInt() { setIntField(m_intField++); }

        int getIntField() { return m_intField; }
        void setIntField(const int value) 
        { 
            if (value != m_intField)
            {
                m_intField = value; 
                notifyPropertyChanged("IntField");
            }
        }

        std::string getStringField() { return m_stringField; }
        void setStringField(const std::string& value)
        { 
            if (value != m_stringField)
            {
                m_stringField = value; 
                notifyPropertyChanged("StringField");
            }
        }  
        
        int m_intField;
        std::string m_stringField;

        private:
        std::unordered_map<std::string, std::list<PropertyChangedEventHandler>> m_eventHandlers;
    };
}