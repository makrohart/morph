#pragma once

#include <memory>
#include <vector>

class JSType
{
    public:
    virtual ~JSType() {};
};

class JSTypes
{
    public:
    static JSTypes* getInstance()
    {
        static JSTypes s_instance;
        return &s_instance;
    }

    void addJSType(std::unique_ptr<JSType> pJSType)
    {
        m_jsTypes.push_back(std::move(pJSType));
    }

    std::vector<std::unique_ptr<JSType>>& getJSTypes()
    {
        return  m_jsTypes;
    }

    private:
    JSTypes() =  default;

    std::vector<std::unique_ptr<JSType>> m_jsTypes;
};