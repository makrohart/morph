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
    inline std::string ToStringFrom()
    {
        throw std::exception("ToStringFrom should be specialized on severity!");
    };

    template<>
    inline std::string ToStringFrom<Severity::Info>()
    {
        return "Info";
    };

    template<>
    inline std::string ToStringFrom<Severity::Warning>()
    {
        return "Warning";
    };

    template<>
    inline std::string ToStringFrom<Severity::Error>()
    {
        return "Error";
    };

    template<>
    inline std::string ToStringFrom<Severity::Fatal>()
    {
        return "Fatal";
    };
}