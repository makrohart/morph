#pragma once

#include <concepts>
#include <string>
#include <type_traits>

#include "v8.h"
#include "JSConverter.h"
#include "../morph/mvvm/ViewModel.h"

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

template<>
struct JSConverter<v8::Local<v8::Value>, mvvm::ViewModel::PropertyChangedEventHandler>
{
    mvvm::ViewModel::PropertyChangedEventHandler toCppFromJS(v8::Local<v8::Value> value)
    {
        if (!value->IsFunction()) {
            throw std::runtime_error("Expected a function");
        }

        v8::Local<v8::Function> func = value.As<v8::Function>();
        auto persistent = std::make_shared<v8::Persistent<v8::Function>>(v8::Isolate::GetCurrent(), func);

        return [persistent]() {
            auto isolate = v8::Isolate::GetCurrent();
            v8::HandleScope scope(isolate);
            auto localFunc = persistent->Get(isolate);
            localFunc->Call(isolate->GetCurrentContext(), v8::Undefined(isolate), 0, nullptr);
        };
    }

    v8::Local<v8::Value> toJSFromCpp(const mvvm::ViewModel::PropertyChangedEventHandler& value)
    {
        auto isolate = v8::Isolate::GetCurrent();
        auto context = isolate->GetCurrentContext();

        auto ext = v8::External::New(isolate, new auto(value));

        return v8::Function::New(
            context, 
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {
                auto ext = v8::Local<v8::External>::Cast(args.Data());
                auto& callback = *static_cast<mvvm::ViewModel::PropertyChangedEventHandler*>(ext->Value());
                callback(); 
            }, 
            ext).ToLocalChecked();
    }
};
