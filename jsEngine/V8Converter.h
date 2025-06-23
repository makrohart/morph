#pragma once

#include <concepts>
#include <string>
#include <type_traits>

#include "v8.h"
#include "JSConverter.h"

// Integer
template<typename CppType>
requires std::is_integral_v<CppType>
struct JSConverter<v8::Local<v8::Value>, CppType>
{
    CppType toCppFromJS(v8::Local<v8::Value> value)
    {
        if constexpr (sizeof(CppType) <= 4)
            return static_cast<CppType>(value->Int32Value(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked());
        else
            return static_cast<CppType>(value->IntegerValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked());
    }

    v8::Local<v8::Value> toJSFromCpp(const CppType value)
    {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(pIsolate);
        if constexpr (sizeof(CppType) <= 4)
            return handle_scope.Escape(v8::Int32::New(pIsolate, value));
        else
            return handle_scope.Escape(v8::Integer::New(pIsolate, value));     
    }
};


template<>
struct JSConverter<v8::Local<v8::Value>, char*>
{
    char* toCppFromJS(v8::Local<v8::Value> value)
    {
        v8::Local<v8::String> v8Str = value.As<v8::String>();
        const int size = v8Str->Utf8Length(v8::Isolate::GetCurrent());
        char* pStr = new char[size + 1] {0};
        v8Str->WriteUtf8(v8::Isolate::GetCurrent(), pStr, size);
        return pStr;
    }

    v8::Local<v8::Value> toJSFromCpp(const char* value)
    {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(pIsolate);
        return handle_scope.Escape(v8::String::NewFromUtf8(pIsolate, value));
    }
};

template<>
struct JSConverter<v8::Local<v8::Value>, std::string>
{
    std::string toCppFromJS(v8::Local<v8::Value> value)
    {
        v8::String::Utf8Value utf8(v8::Isolate::GetCurrent(), value);
        return std::string(*utf8);
    }

    v8::Local<v8::Value> toJSFromCpp(const std::string& value)
    {
        return JSConverter<v8::Local<v8::Value>, char*>().toJSFromCpp(value.c_str());
    }
};
