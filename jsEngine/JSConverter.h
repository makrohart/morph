#pragma once

#include <type_traits>

template<typename JSType, typename CppType>
struct JSConverter
{
    CppType toCppFromJS(JSType value)
    {
        static_assert(false, "Not supported type");
    }

    JSType toJSFromCpp(CppType value)
    {
        return JSConverter<JSType, const CppType>().toJSFromCpp(value);
    }
};

template<typename JSType, typename CppType>
struct JSConverter<JSType, const CppType>
{
    CppType toCppFromJS(JSType value)
    {
        return JSConverter<JSType, CppType>().toCppFromJS(value);
    }

    JSType toJSFromCpp(const CppType value)
    {
        static_assert("Not supported type");
    }
};

template<typename JSType, typename CppType>
struct JSConverter<JSType, const CppType&>
{
    CppType toCppFromJS(JSType value)
    {
        return JSConverter<JSType, CppType>().toCppFromJS(value);
    }

    JSType toJSFromCpp(const CppType& value)
    {
        static_assert("Not supported type");
    }
};

template<typename JSType, typename CppType>
struct JSConverter<JSType, CppType&>
{
    CppType toCppFromJS(JSType value)
    {
        return JSConverter<JSType, CppType>().toCppFromJS(value);
    }

    JSType toJSFromCpp(CppType& value)
    {
        return JSConverter<JSType, const CppType&>().toJSFromCpp(value);
    }
};

template<typename JSType, typename CppType>
struct JSConverter<JSType, CppType&&>
{
    CppType&& toCppFromJS(JSType value)
    {
        return std::move(JSConverter<JSType, CppType>().toCppFromJS(value));
    }

    JSType toJSFromCpp(CppType&& value)
    {
        static_assert("Not supported type");
    }
};

template<typename JSType, typename CppType>
struct JSConverter<JSType, CppType*>
{
    CppType* toCppFromJS(JSType value)
    {
        void* pValue = std::malloc(sizeof(CppType));
        if (!pValue)
            return nullptr;

        new (pValue) CppType(std::move(JSConverter<JSType, CppType>().toCppFromJS(value)));

        return reinterpret_cast<CppType*>(pValue);
    }

    JSType toJSFromCpp(CppType* value)
    {
         return JSConverter<JSType, CppType&>().toJSFromCpp(*value);
    }
};

template<typename JSType, typename CppType>
struct JSConverter<JSType, const CppType*>
{
    CppType* toCppFromJS(JSType value)
    {
        return JSConverter<JSType, CppType*>().toCppFromJS(value);
    }

    JSType toJSFromCpp(const CppType* value)
    {
         return JSConverter<JSType, const CppType&>().toJSFromCpp(*value);
    }
};