#pragma once

#include <algorithm>
#include <type_traits>

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

    template<typename T>
    concept MemberFunctionPointer = std::is_member_function_pointer_v<T>;

    // Concept to check if a pair consists of getter and setter
    template<typename Pair>
    concept GetterSetterPair = requires(Pair p)
    {
        requires MemberFunctionPointer<decltype(p.first)>;
        requires MemberFunctionPointer<decltype(p.second)>;
    };

    struct Type{};
    struct Property{};
    struct Method{};
    struct Field{};
}