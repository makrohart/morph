#pragma once

#include "../needle/Needle.h"
#include "JSType.h"
#include "v8.h"
#include "V8Type.h"
#include "V8Converter.h"

template<needle::FixedString name, auto placeholder, typename C, typename... Args>
struct V8Binding : needle::Needle<name, placeholder, C, Args...>
{
    void visit() override
    {
        v8::FunctionCallback callback = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
             v8::Isolate* pIsolate = info.GetIsolate();

            C* pNative = new C(JSConverter<v8::Local<v8::Value>, Args>().toCppFromJS(info[std::index_sequence_for<Args...>()])...);
            info.This()->SetInternalField(0, v8::External::New(pIsolate, pNative));
        };

        std::unique_ptr<V8Type> pJSType = std::make_unique<V8Type>(name.Str);
        pJSType->addConstructor(callback);
        JSTypes::getInstance()->addJSType(std::move(pJSType));
    }
};

template<needle::FixedString name, auto fieldPtr, typename R, typename C>
struct V8Binding<name, fieldPtr, R C::*> : needle::Needle<name, fieldPtr, R C::*>
{
    void visit() override
    {
    }
};

template<needle::FixedString name, auto accessor, typename R, typename C>
    requires needle::GetterSetterPair<decltype(accessor)>
struct V8Binding<name, accessor, R C::*> : needle::Needle<name, accessor, R C::*>
{
    void visit() override
    {
        v8::AccessorGetterCallback getter = [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info) {
            v8::Isolate* pIsolate = info.GetIsolate();
            v8::Local<v8::Object> self = info.Holder();
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
            C* pNative = static_cast<C*>(native->Value());

            R result = pNative->operator()(accessor.first);
            info.GetReturnValue().Set(JSConverter<v8::Local<v8::Value>, R>().toJSFromCpp(result));
        };

        v8::AccessorSetterCallback setter = [](v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info) {
            v8::Isolate* pIsolate = info.GetIsolate();
            v8::Local<v8::Object> self = info.Holder();
            v8::Local<v8::External> native = v8::Local<v8::External>::Cast(self->GetInternalField(0));
            C* pNative = static_cast<C*>(native->Value());

            pNative->operator()(accessor.second, JSConverter<v8::Local<v8::Value>, R>().toCppFromJS(info.Data()));
        };

        std::vector<std::unique_ptr<JSType>>& jsTypes = JSTypes::getInstance()->getJSTypes();
        std::unique_ptr<JSType>& pJSType = *jsTypes.rbegin();
        V8Type* pV8Type = dynamic_cast<V8Type*>(pJSType.get());
        if (pV8Type)
            pV8Type->addProperty(name.Str, getter, setter);
    }
};

template<needle::FixedString name, auto methodPtr, typename R, typename C, typename... Args>
struct V8Binding<name, methodPtr, R(C::*)(Args...)> : needle::Needle<name, methodPtr, R(C::*)(Args...)>
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
                    pNative->operator()(methodPtr, JSConverter<v8::Local<v8::Value>, Args>().toCppFromJS(info[Is])...);
                }(std::index_sequence_for<Args...>{});
            } 
            else 
            {
                R result = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return pNative->operator()(methodPtr, JSConverter<v8::Local<v8::Value>, Args>().toCppFromJS(info[Is])...);
                }(std::index_sequence_for<Args...>{});
                info.GetReturnValue().Set(JSConverter<v8::Local<v8::Value>, R>().toJSFromCpp(result));
            }
        };

        std::vector<std::unique_ptr<JSType>>& jsTypes = JSTypes::getInstance()->getJSTypes();
        std::unique_ptr<JSType>& pJSType = *jsTypes.rbegin();
        V8Type* pV8Type = dynamic_cast<V8Type*>(pJSType.get());
        if (pV8Type)
            pV8Type->addMethod(name.Str, callback);
    }
};
