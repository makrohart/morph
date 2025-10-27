#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <type_traits>

#include "v8.h"
#include "../eventable/EventArgs.h"

template<typename T>
struct bridge_cast;

template<typename Target>
struct bridge_cast<Target(const v8::Local<v8::Value>&)>;

template<typename Target>
struct bridge_cast<const Target(const v8::Local<v8::Value>&)>
{
    Target operator()(const v8::Local<v8::Value>& source)
    {
        return bridge_cast<Target(const v8::Local<v8::Value>&)>()(source);
    }
};

template<typename Target>
struct bridge_cast<Target&(const v8::Local<v8::Value>&)>;

template<typename Target>
struct bridge_cast<const Target&(const v8::Local<v8::Value>&)>
{
    Target& operator()(const v8::Local<v8::Value>& source)
    {
        return bridge_cast<Target&(const v8::Local<v8::Value>&)>()(source);
    }
};

template<typename Target>
struct bridge_cast<Target*(const v8::Local<v8::Value>&)>;

template<typename Target>
struct bridge_cast<const Target*(const v8::Local<v8::Value>&)>
{
    Target* operator()(const v8::Local<v8::Value>& source)
    {
        return bridge_cast<Target*(const v8::Local<v8::Value>&)>()(source);
    }
};

template<typename Target>
    requires std::is_integral_v<Target>
struct bridge_cast<Target(const v8::Local<v8::Value>&)>
{
    Target operator()(const v8::Local<v8::Value>& source)
    {
        if constexpr (sizeof(Target) <= 4)
            return static_cast<Target>(source->Int32Value(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked());
        else
            return static_cast<Target>(source->IntegerValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked());
    }
};

template<>
struct bridge_cast<double(const v8::Local<v8::Value>&)>
{
    double operator()(const v8::Local<v8::Value>& source)
    {
        source->NumberValue(v8::Isolate::GetCurrent()->GetCurrentContext()).ToChecked();
    }
};

template<>
struct bridge_cast<char(const v8::Local<v8::Value>&)>
{
    char operator()(const v8::Local<v8::Value>& source)
    {
        if (source->IsString()) {
            v8::String::Utf8Value utf8(v8::Isolate::GetCurrent(), source);
            if (*utf8 && utf8.length() > 0) {
                return (*utf8)[0]; // Return first character
            }
        }
        return '\0'; // Default if conversion fails
    }
};

template<>
struct bridge_cast<char*(const v8::Local<v8::Value>&)>
{
    char* operator()(const v8::Local<v8::Value>& source)
    {
        v8::Local<v8::String> str = source.As<v8::String>();
        const int size = str->Utf8Length(v8::Isolate::GetCurrent());
        char* m_pStr = new char[size + 1] {0};
        str->WriteUtf8(v8::Isolate::GetCurrent(), m_pStr, size);
        return m_pStr;
    }

    ~bridge_cast() { if (m_pStr) delete m_pStr; }

    private:
    char* m_pStr;
};

template<>
struct bridge_cast<std::string&((const v8::Local<v8::Value>&))>
{
    std::string& operator()(const v8::Local<v8::Value>& source)
    {
        v8::String::Utf8Value utf8(v8::Isolate::GetCurrent(), source);
        m_str = new std::string(*utf8);
        return *m_str;
    }

    // Let memory leak here as it is passed as referenced arguement 
    // ~bridge_cast() { if (m_str) delete m_str; }

    private:
    std::string* m_str;
};

template<>
struct bridge_cast<std::function<void()>&(const v8::Local<v8::Value>&)>
{
    std::function<void()>& operator()(const v8::Local<v8::Value>& source)
    {
        if (!source->IsFunction())
            static_assert("Expected a function");

        v8::Local<v8::Function> func = source.As<v8::Function>();
        auto persistent = std::make_shared<v8::Persistent<v8::Function>>(v8::Isolate::GetCurrent(), func);

        // Heap allocated as bridge_cast might be used as rvalue, the returned reference has stable no address
        m_pFunc = new std::function<void()>([persistent]() {
            auto isolate = v8::Isolate::GetCurrent();
            v8::HandleScope scope(isolate);
            auto localFunc = persistent->Get(isolate);
            localFunc->Call(isolate->GetCurrentContext(), v8::Undefined(isolate), 0, nullptr);
        });

        return *m_pFunc;
    }

    // Let memory leak here as it is passed as referenced arguement 
    // ~bridge_cast() { if (m_pFunc) delete m_pFunc; }

    private:
    std::function<void()>* m_pFunc;
};

template<>
struct bridge_cast<std::function<void(eventable::EventArgs)>&(const v8::Local<v8::Value>&)>
{
    std::function<void(eventable::EventArgs)>& operator()(const v8::Local<v8::Value>& source)
    {
        if (!source->IsFunction())
            static_assert("Expected a function");

        v8::Local<v8::Function> func = source.As<v8::Function>();
        auto persistent = std::make_shared<v8::Persistent<v8::Function>>(v8::Isolate::GetCurrent(), func);
        auto persistent_ctx = std::make_shared<v8::Persistent<v8::Context>>(v8::Isolate::GetCurrent(), v8::Isolate::GetCurrent()->GetCurrentContext());
        // Heap allocated as bridge_cast might be used as rvalue, the returned reference has stable no address
        m_pFunc = new std::function<void(eventable::EventArgs)>([persistent, pIsolate = v8::Isolate::GetCurrent(), persistent_ctx](eventable::EventArgs eventArgs) {
            v8::Isolate::Scope isolate_scope(pIsolate);
            v8::HandleScope scope(pIsolate);
            v8::Local<v8::Context> context = persistent_ctx->Get(pIsolate);
            v8::Context::Scope context_scope(context);

            auto localFunc = persistent->Get(pIsolate);
            localFunc->Call(context, v8::Undefined(pIsolate), 0, nullptr);
        });

        return *m_pFunc;
    }

    // Let memory leak here as it is passed as referenced arguement 
    // ~bridge_cast() { if (m_pFunc) delete m_pFunc; }

    private:
    std::function<void(eventable::EventArgs)>* m_pFunc;
};

template<typename Target>
    requires std::is_class<Target>::value
struct bridge_cast<Target&(const v8::Local<v8::Value>&)>
{
    Target& operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(pIsolate);

        if (!source->IsObject())
        {
            pIsolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(pIsolate, "Expected a class object")));
            static_assert("Expected a class object");
        }

        v8::Local<v8::Object> obj = source.As<v8::Object>();
        if (obj->InternalFieldCount() > 0)
        {
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(obj->GetInternalField(0));
            Target* pNative = static_cast<Target*>(native->Value());
            return *pNative;
        }

        pIsolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(pIsolate, "Expected a class object")));
        static_assert("Expected a class object");
    }
};

template<typename Target>
    requires std::is_class<Target>::value
struct bridge_cast<Target*(const v8::Local<v8::Value>&)>
{
    Target* operator()(const v8::Local<v8::Value>& source)
    {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent();
        v8::HandleScope scope(pIsolate);

        if (!source->IsObject())
        {
            pIsolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(pIsolate, "Expected a class object")));
            static_assert("Expected a class object");
        }

        v8::Local<v8::Object> obj = source.As<v8::Object>();
        if (obj->InternalFieldCount() > 0)
        {
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(obj->GetInternalField(0));
            Target* pNative = static_cast<Target*>(native->Value());
            return pNative;
        }

        pIsolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(pIsolate, "Expected a class object")));
        static_assert("Expected a class object");
    }
};




template<typename Source>
struct bridge_cast<v8::Local<v8::Value>(Source)>
{
    v8::Local<v8::Value> operator()(Source source)
    {
        return bridge_cast<v8::Local<v8::Value>(const Source)>()(source);
    }
};

template<typename Source>
struct bridge_cast<v8::Local<v8::Value>(const Source)>;

template<typename Source>
struct bridge_cast<v8::Local<v8::Value>(Source&)>
{
    v8::Local<v8::Value> operator()(Source& source)
    {
        return bridge_cast<v8::Local<v8::Value>(const Source&)>()(source);
    }
};

template<typename Source>
struct bridge_cast<v8::Local<v8::Value>(const Source&)>;


template<typename Source>
struct bridge_cast<v8::Local<v8::Value>(Source*)>
{
    v8::Local<v8::Value> operator()(Source* source)
    {
        return bridge_cast<v8::Local<v8::Value>(const Source*)>()(source);
    }
};

template<typename Source>
struct bridge_cast<v8::Local<v8::Value>(const Source*)>;

template<typename Source>
    requires std::is_integral_v<Source>
struct bridge_cast<v8::Local<v8::Value>(const Source)>
{
    v8::Local<v8::Value> operator()(const Source source)
    {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(pIsolate);
        if constexpr (sizeof(Source) <= 4)
            return handle_scope.Escape(v8::Int32::New(pIsolate, source));
        else
            return handle_scope.Escape(v8::Integer::New(pIsolate, source));
    }
};

template<>
struct bridge_cast<v8::Local<v8::Value>(const char)>
{
    v8::Local<v8::Value> operator()(const char source)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        char str[2] = {source, '\0'};
        return v8::String::NewFromUtf8(isolate, str);
    }
};

template<>
struct bridge_cast<v8::Local<v8::Value>(const char*)>
{
    v8::Local<v8::Value> operator()(const char* source)
    {
        v8::Isolate* pIsolate = v8::Isolate::GetCurrent();
        v8::EscapableHandleScope handle_scope(pIsolate);
        return handle_scope.Escape(v8::String::NewFromUtf8(pIsolate, source));
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

template<>
struct bridge_cast<v8::Local<v8::Value>(const std::string)>
{
    v8::Local<v8::Value> operator()(const std::string source)
    {
        return bridge_cast<v8::Local<v8::Value>(const char*)>()(source.c_str());
    }
};

template<>
struct bridge_cast<v8::Local<v8::Value>(const std::function<void()>&)>
{
    v8::Local<v8::Value> operator()(const std::function<void()>& source)
    {
        auto isolate = v8::Isolate::GetCurrent();
        auto context = isolate->GetCurrentContext();

        auto ext = v8::External::New(isolate, new auto(source));

        return v8::Function::New(
            context, 
            [](const v8::FunctionCallbackInfo<v8::Value>& args) {
                auto ext = v8::Local<v8::External>::Cast(args.Data());
                auto& callback = *static_cast<std::function<void()>*>(ext->Value());
                callback(); 
            }, 
            ext).ToLocalChecked();
    }
};

// TODO: support class-like bridge_cast
template<typename Source>
    requires std::is_class<Source>::value
struct bridge_cast<v8::Local<v8::Value>(const Source&)>;