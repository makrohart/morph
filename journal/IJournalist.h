#pragma once

#include <string>

namespace journal
{
    class IJournalist
    {
        public:
        virtual void write(const std::string& data) = 0;
        virtual void flush() = 0;
    };
}