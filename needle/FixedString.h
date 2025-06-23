#pragma once

#include <algorithm>

namespace needle
{
    template<size_t N>
    struct FixedString {
        char Str[N+1]{0};
        
        constexpr FixedString(const char(&s)[N])
        {
            std::copy_n(s, N, Str);
        }
    };
}