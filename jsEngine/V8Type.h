#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "JSType.h"
#include "v8.h"

class V8Type : public JSType
{
    public:
    V8Type(const std::string& name) : m_name(name) {}

    void addConstructor(v8::FunctionCallback callback);
    void addMethod(const std::string& name, v8::FunctionCallback callback);
    void addProperty(const std::string& name, v8::AccessorGetterCallback getterCallback = nullptr, v8::AccessorSetterCallback setterCallback = nullptr);

    const std::string getName() const { return m_name; }
    v8::FunctionCallback getConstructor() const;
    const std::unordered_map<std::string, v8::FunctionCallback>& getMethods() const;
    const std::unordered_map<std::string, std::pair<v8::AccessorGetterCallback, v8::AccessorSetterCallback>>& getProperties() const;

    private:
    const std::string m_name;
    v8::FunctionCallback m_constructor;
    std::unordered_map<std::string, v8::FunctionCallback> m_methods;
    std::unordered_map<std::string, std::pair<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> m_accessors;
};