#pragma once

#include <string>

class JSEngine
{
    public:
    virtual void createContext() = 0;

    virtual void compile(const std::string& script) = 0;

    virtual void execute(const std::string& script) = 0;
};