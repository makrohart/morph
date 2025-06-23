#pragma once

#include "JSEngine.h"
#include "v8.h"

class V8Engine : public JSEngine
{
    public:
    V8Engine();
    ~V8Engine();

    void createContext() override;
    void compile(const std::string& script) override;
    void execute(const std::string& script) override;
    private:
    std::unique_ptr<v8::Platform> m_upPlatform;
    v8::Isolate::CreateParams m_createParams;
    v8::Isolate* m_pIsolate;
    v8::Global<v8::Context> m_context;
};