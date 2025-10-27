#include "V8Type.h"

void V8Type::addConstructor(v8::FunctionCallback callback)
{
    m_constructor = callback;     
}

void V8Type::addMethod(const std::string& name, v8::FunctionCallback callback)
{
    if (m_methods.find(name) != m_methods.cend())
        return;

    m_methods[name] = callback;
}

void V8Type::addProperty(const std::string& name, v8::AccessorGetterCallback getterCallback /*= nullptr*/, v8::AccessorSetterCallback setterCallback /*= nullptr*/)
{
    if (m_accessors.find(name) == m_accessors.cend())
    {
        m_accessors[name] = std::make_pair(getterCallback, setterCallback);
        return;
    }

    if (getterCallback)
        m_accessors[name].first = getterCallback;

    if (setterCallback)
        m_accessors[name].second = setterCallback;

}

v8::FunctionCallback V8Type::getConstructor() const
{
    return m_constructor;
}

const std::unordered_map<std::string, v8::FunctionCallback>& V8Type::getMethods() const
{
    return m_methods;
}

const std::unordered_map<std::string, std::pair<v8::AccessorGetterCallback, v8::AccessorSetterCallback>>& V8Type::getProperties() const
{
    return m_accessors;
}