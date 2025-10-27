#pragma once

#include "../JSEngine.h"
#include "v8.h"

class V8Engine : public JSEngine
{
    public:
    V8Engine();
    ~V8Engine();

    void createContext() override;
    bool compile(const std::string& script) override;
    bool execute(const std::string& script) override;

    private:
    void proccessException(v8::TryCatch* pTryCatch);

    private:
    std::unique_ptr<v8::Platform> m_upPlatform;
    v8::Isolate::CreateParams m_createParams;
    v8::Isolate* m_pIsolate;
    v8::Global<v8::Context> m_context;
    v8::Global<v8::Script> m_script;
};