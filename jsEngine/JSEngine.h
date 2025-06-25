#pragma once

#include <string>

class JSEngine
{
    public:
    inline void run(const std::string& script)
    {
        createContext();
        if (!compile(script))
            return;

        execute(script);
    }

    virtual void createContext() = 0;
    virtual bool compile(const std::string& script) = 0;
    virtual bool execute(const std::string& script) = 0;
};