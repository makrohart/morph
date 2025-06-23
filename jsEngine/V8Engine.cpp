#include "V8Engine.h"

#include "libplatform/libplatform.h"
#include "V8Type.h"

V8Engine::V8Engine()
{
    // icu_data_file, natives_blob.bin and snapshot_blob.bin 
    // locate under the same directory with build directory.
    v8::V8::InitializeICUDefaultLocation("");
    v8::V8::InitializeExternalStartupData("");

    m_upPlatform = v8::platform::NewDefaultPlatform();

    v8::V8::InitializePlatform(m_upPlatform.get());
    if (!v8::V8::Initialize())
        throw std::exception("Failed to initialize V8 engine.");
}

V8Engine::~V8Engine()
{
	m_pIsolate->Dispose();
	v8::V8::Dispose();
	// v8::V8::DisposePlatform();
	delete m_createParams.array_buffer_allocator;
}

void V8Engine::createContext()
{
    v8::Isolate::Scope isolate_scope(m_pIsolate);

    // Create a template for the global object.
	v8::Local<v8::ObjectTemplate> globalTemplate = v8::ObjectTemplate::New(m_pIsolate);

	std::stack<JSType>& jsTypes = JSTypes::getInstance()->getJSTypes();

	// Bind Class
    while (!jsTypes.empty())
    {
        const JSType& jsType = jsTypes.top();
        const V8Type* pV8Type = dynamic_cast<const V8Type*>(&jsType);
        if (!pV8Type)
        {
            jsTypes.pop();
            continue;
        }

        const std::string& className = pV8Type->getName();
		const auto& constructor = pV8Type->getConstructor();
        const auto& methods = pV8Type->getMethods();
        const auto& properties = pV8Type->getProperties();

		// Bind constructor

		v8::Local<v8::FunctionTemplate> classTemplate = v8::FunctionTemplate::New(m_pIsolate, constructor);
		classTemplate->SetClassName(v8::String::NewFromUtf8(m_pIsolate, className.c_str()));
		globalTemplate->Set(m_pIsolate, className.c_str(), classTemplate);
		v8::Local<v8::ObjectTemplate> classPrototype = classTemplate->PrototypeTemplate();

		// Bind methods
		for (const auto& [methodName, method] : methods)
            classPrototype->Set(m_pIsolate, methodName.c_str(), v8::FunctionTemplate::New(m_pIsolate, method));

		v8::Local<v8::ObjectTemplate> classInstance = classTemplate->InstanceTemplate();
		classInstance->SetInternalFieldCount(1);

		// Bind getters and setters
		for (const auto& [propertyName, accessor] : properties)
			classInstance->SetNativeDataProperty(v8::String::NewFromUtf8(m_pIsolate, propertyName.c_str()), accessor.first, accessor.second);

        jsTypes.pop();
    };
    
	m_context = v8::Global<v8::Context>(m_pIsolate, v8::Context::New(m_pIsolate, nullptr, globalTemplate));
}

void V8Engine::compile(const std::string& script)
{

}

void V8Engine::execute(const std::string& script)
{

}