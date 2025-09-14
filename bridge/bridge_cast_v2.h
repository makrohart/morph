#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <type_traits>
#include <memory>
#include <array>

#include "v8.h"
#include "../eventable/EventArgs.h"

namespace bridge_cast_v2 {

// Main bridge_cast template
template<typename T>
struct bridge_cast;

// V8 to C++ conversions

// Integral types
template<typename Target>
    requires std::is_integral_v<Target>
struct bridge_cast<Target(const v8::Local<v8::Value>&)>
{
    Target operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        
        if constexpr (sizeof(Target) <= 4)
            return static_cast<Target>(source->Int32Value(isolate->GetCurrentContext()).FromJust());
        else
            return static_cast<Target>(source->IntegerValue(isolate->GetCurrentContext()).FromJust());
    }
};

// Double type
template<>
struct bridge_cast<double(const v8::Local<v8::Value>&)>
{
    double operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        return source->NumberValue(isolate->GetCurrentContext()).FromJust();
    }
};

// Char type
template<>
struct bridge_cast<char(const v8::Local<v8::Value>&)>
{
    char operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        
        if (source->IsString()) {
            v8::String::Utf8Value utf8(isolate, source);
            if (*utf8 && utf8.length() > 0) {
                return (*utf8)[0];
            }
        }
        return '\0';
    }
};

// String types
template<>
struct bridge_cast<std::string(const v8::Local<v8::Value>&)>
{
    std::string operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        
        v8::String::Utf8Value utf8(isolate, source);
        return std::string(*utf8);
    }
};

// Function types
template<>
struct bridge_cast<std::function<void()>(const v8::Local<v8::Value>&)>
{
    std::function<void()> operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        
        if (!source->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(
                v8::String::NewFromUtf8(isolate, "Expected a function")));
            return {};
        }

        v8::Local<v8::Function> func = source.As<v8::Function>();
        auto persistent = std::make_shared<v8::Persistent<v8::Function>>(isolate, func);

        return [persistent]() {
            auto isolate = v8::Isolate::GetCurrent();
            v8::HandleScope scope(isolate);
            auto localFunc = persistent->Get(isolate);
            localFunc->Call(isolate->GetCurrentContext(), v8::Undefined(isolate), 0, nullptr);
        };
    }
};

template<>
struct bridge_cast<std::function<void(eventable::EventArgs)>(const v8::Local<v8::Value>&)>
{
    std::function<void(eventable::EventArgs)> operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);
        
        if (!source->IsFunction()) {
            isolate->ThrowException(v8::Exception::TypeError(
                v8::String::NewFromUtf8(isolate, "Expected a function")));
            return {};
        }

        v8::Local<v8::Function> func = source.As<v8::Function>();
        auto persistent = std::make_shared<v8::Persistent<v8::Function>>(isolate, func);
        auto persistent_ctx = std::make_shared<v8::Persistent<v8::Context>>(isolate, isolate->GetCurrentContext());
        
        return [persistent, isolate, persistent_ctx](eventable::EventArgs eventArgs) {
            v8::Isolate::Scope isolate_scope(isolate);
            v8::HandleScope scope(isolate);
            v8::Local<v8::Context> context = persistent_ctx->Get(isolate);
            v8::Context::Scope context_scope(context);

            auto localFunc = persistent->Get(isolate);
            localFunc->Call(context, v8::Undefined(isolate), 0, nullptr);
        };
    }
};

// Class types - return pointer to avoid copy constructor issues
template<typename Target>
    requires std::is_class_v<Target>
struct bridge_cast<Target(const v8::Local<v8::Value>&)>
{
    Target operator()(const v8::Local<v8::Value>& source)
    {
        Target* pNative = bridge_cast<Target*(const v8::Local<v8::Value>&)>()(source);
        return *pNative;
    }
};

// Reference types return T* for classes, T for non-classes
template<typename Target>
struct bridge_cast<Target&(const v8::Local<v8::Value>&)>
{
    auto operator()(const v8::Local<v8::Value>& source) -> decltype(bridge_cast<Target(const v8::Local<v8::Value>&)>()(source))
    {
        return bridge_cast<Target(const v8::Local<v8::Value>&)>()(source);
    }
};

template<typename Target>
struct bridge_cast<const Target&(const v8::Local<v8::Value>&)>
{
    auto operator()(const v8::Local<v8::Value>& source) -> decltype(bridge_cast<Target(const v8::Local<v8::Value>&)>()(source))
    {
        return bridge_cast<Target(const v8::Local<v8::Value>&)>()(source);
    }
};

// Const value types return T* for classes, T for non-classes
template<typename Target>
struct bridge_cast<const Target(const v8::Local<v8::Value>&)>
{
    auto operator()(const v8::Local<v8::Value>& source) -> decltype(bridge_cast<Target(const v8::Local<v8::Value>&)>()(source))
    {
        return bridge_cast<Target(const v8::Local<v8::Value>&)>()(source);
    }
};

// Pointer types return T*
template<typename Target>
struct bridge_cast<Target*(const v8::Local<v8::Value>&)>
{
    Target* operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(isolate);

        if (!source->IsObject()) {
            isolate->ThrowException(v8::Exception::TypeError(
                v8::String::NewFromUtf8(isolate, "Expected a class object")));
            return nullptr;
        }

        v8::Local<v8::Object> obj = source.As<v8::Object>();
        if (obj->InternalFieldCount() > 0) {
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(obj->GetInternalField(0));
            Target* pNative = static_cast<Target*>(native->Value());
            return pNative;
        }

        isolate->ThrowException(v8::Exception::TypeError(
            v8::String::NewFromUtf8(isolate, "Expected a class object")));
        return nullptr;
    }
};

template<typename Target>
struct bridge_cast<const Target*(const v8::Local<v8::Value>&)>
{
    Target* operator()(const v8::Local<v8::Value>& source)
    {
        return bridge_cast<Target(const v8::Local<v8::Value>&)>()(source);
    }
};

// C++ to V8 conversions

// Integral types
template<typename Source>
    requires std::is_integral_v<Source>
struct bridge_cast<v8::Local<v8::Value>(Source)>
{
    v8::Local<v8::Value> operator()(Source source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(isolate);
        
        if constexpr (sizeof(Source) <= 4)
            return handle_scope.Escape(v8::Int32::New(isolate, source));
        else
            return handle_scope.Escape(v8::Integer::New(isolate, source));
    }
};

// Char type
template<>
struct bridge_cast<v8::Local<v8::Value>(char)>
{
    v8::Local<v8::Value> operator()(char source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        char str[2] = {source, '\0'};
        return v8::String::NewFromUtf8(isolate, str);
    }
};

// Double type
template<>
struct bridge_cast<v8::Local<v8::Value>(double)>
{
    v8::Local<v8::Value> operator()(double source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        return v8::Number::New(isolate, source);
    }
};

// String types
template<>
struct bridge_cast<v8::Local<v8::Value>(const char*)>
{
    v8::Local<v8::Value> operator()(const char* source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(isolate);
        return handle_scope.Escape(v8::String::NewFromUtf8(isolate, source));
    }
};

template<>
struct bridge_cast<v8::Local<v8::Value>(std::string)>
{
    v8::Local<v8::Value> operator()(std::string source)
    {
        return bridge_cast<v8::Local<v8::Value>(const char*)>()(source.c_str());
    }
};

template<>
struct bridge_cast<v8::Local<v8::Value>(const std::string&)>
{
    v8::Local<v8::Value> operator()(const std::string& source)
    {
        return bridge_cast<v8::Local<v8::Value>(const char*)>()(source.c_str());
    }
};

// Function types
template<>
struct bridge_cast<v8::Local<v8::Value>(std::function<void()>)>
{
    v8::Local<v8::Value> operator()(std::function<void()> source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(isolate);
        auto context = isolate->GetCurrentContext();

        // Use smart pointer to avoid memory leaks
        auto callback_ptr = std::make_shared<std::function<void()>>(std::move(source));
        auto ext = v8::External::New(isolate, callback_ptr.get());

        return handle_scope.Escape(v8::Function::New(
            context, 
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {
                auto ext = v8::Local<v8::External>::Cast(args.Data());
                auto* callback = static_cast<std::function<void()>*>(ext->Value());
                (*callback)(); 
            }, 
            ext).ToLocalChecked());
    }
};

// Class types - placeholder implementations
// Note: These are placeholders. The actual V8 binding system should handle object wrapping
template<typename Source>
    requires std::is_class_v<Source>
struct bridge_cast<v8::Local<v8::Value>(const Source&)>
{
    v8::Local<v8::Value> operator()(const Source& source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        // Placeholder - the V8 binding system should handle actual object wrapping
        return v8::Undefined(isolate);
    }
};

template<typename Source>
    requires std::is_class_v<Source>
struct bridge_cast<v8::Local<v8::Value>(Source*)>
{
    v8::Local<v8::Value> operator()(Source* source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        // Placeholder - the V8 binding system should handle actual object wrapping
        return v8::Undefined(isolate);
    }
};

template<typename Source>
    requires std::is_class_v<Source>
struct bridge_cast<v8::Local<v8::Value>(const Source*)>
{
    v8::Local<v8::Value> operator()(const Source* source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        // Placeholder - the V8 binding system should handle actual object wrapping
        return v8::Undefined(isolate);
    }
};

} // namespace bridge_cast_v2

// Alias for backward compatibility
namespace bridge_cast = bridge_cast_v2;