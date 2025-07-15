#pragma once

#include "../../needle/Needle.h"
#include "../JSType.h"
#include "v8.h"
#include "V8Type.h"
#include "../bridge_cast.h"

template<needle::FixedString name, auto placeholder, typename C, typename... Args>
struct V8Bridge : needle::Needle<name, placeholder, C, Args...>
{
    void visit() override
    {
        v8::FunctionCallback callback = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
             v8::Isolate* pIsolate = info.GetIsolate();

            C* pNative = new C(bridge_cast<Args(const v8::Local<v8::Value>&)>{}(info[std::index_sequence_for<Args...>()])...);
            info.This()->SetInternalField(0, v8::External::New(pIsolate, pNative));
        };

        std::unique_ptr<V8Type> pJSType = std::make_unique<V8Type>(name.Str);
        pJSType->addConstructor(callback);
        JSTypes::getInstance()->addJSType(std::move(pJSType));
    }
};

template<needle::FixedString name, auto fieldPtr, typename R, typename C>
struct V8Bridge<name, fieldPtr, R C::*> : needle::Needle<name, fieldPtr, R C::*>
{
    void visit() override
    {
    }
};

template<needle::FixedString name, auto getterPtr, typename R, typename C, typename T>
struct V8Bridge<name, getterPtr, R(C::*)(), T> : needle::Needle<name, getterPtr, R(C::*)(), T>
{
    void visit() override
    {
        v8::AccessorGetterCallback getter = [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
            v8::Isolate* pIsolate = info.GetIsolate();
            v8::Local<v8::Object> self = info.Holder();
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
            C* pNative = static_cast<C*>(native->Value());

            R result = pNative->operator()(getterPtr);
            info.GetReturnValue().Set(bridge_cast<v8::Local<v8::Value>(R)>{}(result));
        };

        std::vector<std::unique_ptr<JSType>>& jsTypes = JSTypes::getInstance()->getJSTypes();
        std::unique_ptr<JSType>& pJSType = *jsTypes.rbegin();
        V8Type* pV8Type = dynamic_cast<V8Type*>(pJSType.get());
        if (pV8Type)
            pV8Type->addProperty(name.Str, getter, nullptr);
    }
};

template<needle::FixedString name, auto setterPtr, typename R, typename C, typename T>
struct V8Bridge<name, setterPtr, void(C::*)(R), T> : needle::Needle<name, setterPtr, void(C::*)(R), T>
{
    void visit() override
    {
        v8::AccessorSetterCallback setter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
            v8::Isolate* pIsolate = info.GetIsolate();
            v8::Local<v8::Object> self = info.Holder();
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
            C* pNative = static_cast<C*>(native->Value());

            pNative->template operator()<void, C, R>(setterPtr, bridge_cast<R(const v8::Local<v8::Value>&)>{}(info.Data()));
        };

        std::vector<std::unique_ptr<JSType>>& jsTypes = JSTypes::getInstance()->getJSTypes();
        std::unique_ptr<JSType>& pJSType = *jsTypes.rbegin();
        V8Type* pV8Type = dynamic_cast<V8Type*>(pJSType.get());
        if (pV8Type)
            pV8Type->addProperty(name.Str, nullptr, setter);
    }
};

template<needle::FixedString name, auto methodPtr, typename R, typename C, typename... Args>
struct V8Bridge<name, methodPtr, R(C::*)(Args...)> : needle::Needle<name, methodPtr, R(C::*)(Args...)>
{
    void visit() override
    {
        v8::FunctionCallback callback = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
            v8::Isolate* pIsolate = info.GetIsolate();
            v8::HandleScope handleScope(pIsolate);
            v8::Local<v8::Object> self = info.Holder();
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
            C* pNative = static_cast<C*>(native->Value());
            
            if constexpr (std::is_void_v<R>) 
            {
                [&]<std::size_t... Is>(std::index_sequence<Is...>){
                    pNative->template operator()<R, C, Args...>(methodPtr, bridge_cast<Args(const v8::Local<v8::Value>&)>{}(info[Is])...);
                }(std::index_sequence_for<Args...>{});
            } 
            else 
            {
                R result = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return pNative->template operator()<R, C, Args...>(methodPtr, bridge_cast<Args(const v8::Local<v8::Value>&)>{}(info[Is])...);
                }(std::index_sequence_for<Args...>{});
                info.GetReturnValue().Set(bridge_cast<v8::Local<v8::Value>(R)>{}(result));
            }
        };

        std::vector<std::unique_ptr<JSType>>& jsTypes = JSTypes::getInstance()->getJSTypes();
        std::unique_ptr<JSType>& pJSType = *jsTypes.rbegin();
        V8Type* pV8Type = dynamic_cast<V8Type*>(pJSType.get());
        if (pV8Type)
            pV8Type->addMethod(name.Str, callback);
    }
};
