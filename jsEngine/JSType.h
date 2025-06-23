#pragma once

#include <stack>

class JSType
{
    public:
    virtual ~JSType() = default;
};

class JSTypes
{
    public:
    static JSTypes* getInstance()
    {
        static JSTypes s_instance;
        return &s_instance;
    }

    void addJSType(JSType&& jsType)
    {
        m_jsTypes.push(jsType);
    }

    std::stack<JSType>& getJSTypes()
    {
        return  m_jsTypes;
    }

    private:
    JSTypes() =  default;

    std::stack<JSType> m_jsTypes;
};