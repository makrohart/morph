#pragma once

#include <string>

namespace journal
{
    enum class Severity
    {
        Info = 0,
        Warning = 1,
        Error = 2,
        Fatal = 3
    };

    template<Severity severity>
    std::string ToStringFrom()
    {
        throw std::exception("ToStringFrom should be specialized on severity!");
    };

    template<>
    std::string ToStringFrom<Severity::Info>()
    {
        return "Info";
    };

    template<>
    std::string ToStringFrom<Severity::Warning>()
    {
        return "Warning";
    };

    template<>
    std::string ToStringFrom<Severity::Error>()
    {
        return "Error";
    };

    template<>
    std::string ToStringFrom<Severity::Fatal>()
    {
        return "Fatal";
    };
}