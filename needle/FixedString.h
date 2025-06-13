#pragma once

#include <algorithm>

namespace needle
{
    template<size_t N>
    struct FixedString {
        int str[N];
        
        constexpr FixedString(const char(&s)[N])
        {
            std::copy_n(s, N, str);
        }
    };
}