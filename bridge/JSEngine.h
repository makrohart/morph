#pragma once

#include <string>
#include <vector>

class JSEngine
{
    public:
    inline void run(const std::vector<std::string>& scripts)
    {
        createContext();

        for (const auto& script : scripts)
        {
            if (!compile(script))
                return;

            execute(script);
        }
    }

    virtual void createContext() = 0;
    virtual bool compile(const std::string& script) = 0;
    virtual bool execute(const std::string& script) = 0;
};