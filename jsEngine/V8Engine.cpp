#include "V8Engine.h"

#include "libplatform/libplatform.h"
#include "../journal/Journal.h"
#include "../journal/Severity.h"
#include "V8Type.h"

namespace
{
	v8::MaybeLocal<v8::String> ReadFile(v8::Isolate* isolate, const char* name)
	{
#pragma warning(suppress : 4996)
		FILE* file = fopen(name, "rb");
		if (file == nullptr) return {};

		fseek(file, 0, SEEK_END);
		size_t size = ftell(file);
		rewind(file);

		char* chars = new char[size + 1];
		chars[size] = '\0';
		for (size_t i = 0; i < size;) {
			i += fread(&chars[i], 1, size - i, file);
			if (ferror(file)) {
				fclose(file);
				return {};
			}
		}
		fclose(file);
		v8::MaybeLocal<v8::String> result = v8::String::NewFromUtf8(isolate, chars, v8::NewStringType::kNormal, static_cast<int>(size));
		delete[] chars;
		return result;
	}
}

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

	m_createParams.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
	m_pIsolate = v8::Isolate::New(m_createParams);
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
    v8::HandleScope handle_scope(m_pIsolate);

    // Create a template for the global object.
	v8::Local<v8::ObjectTemplate> globalTemplate = v8::ObjectTemplate::New(m_pIsolate);

	std::vector<std::unique_ptr<JSType>>& jsTypes = JSTypes::getInstance()->getJSTypes();

	// Bind Class
	for (std::unique_ptr<JSType>& pJSType : jsTypes)
    {
        V8Type* pV8Type = dynamic_cast<V8Type*>(pJSType.get());
        if (!pV8Type)
		 	continue;

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
			
    };
    
	m_context = v8::Global<v8::Context>(m_pIsolate, v8::Context::New(m_pIsolate, nullptr, globalTemplate));
}

bool V8Engine::compile(const std::string& script)
{
	v8::Isolate::Scope isolate_scope(m_pIsolate);
    v8::HandleScope handle_scope(m_pIsolate);
    bool success = true;
    {
        // Enter the execution environment before evaluating any code.
        v8::HandleScope handle_scope(m_pIsolate);
        v8::Context::Scope context_scope(m_context.Get(m_pIsolate));
        v8::Local<v8::String> file_name = v8::String::NewFromUtf8(m_pIsolate, script.c_str());
        v8::Local<v8::String> source;
        if (!ReadFile(m_pIsolate, script.c_str()).ToLocal(&source))
			journal::Journal<journal::Severity::Fatal>() <<"Error reading "<< script.c_str();

		v8::TryCatch try_catch(m_pIsolate);
		v8::ScriptOrigin origin(file_name);
		v8::Local<v8::Context> context(m_pIsolate->GetCurrentContext());
		v8::Local<v8::Script> v8Script;
		if (!v8::Script::Compile(context, source, &origin).ToLocal(&v8Script))
		{
			// Print errors that happened during compilation.
			proccessException(&try_catch);
			success = false;
		}
        m_script = v8::Global<v8::Script>(m_pIsolate, v8Script);
		
		return success;
    }
}

bool V8Engine::execute(const std::string& script)
{
	v8::Isolate::Scope isolate_scope(m_pIsolate);
    v8::HandleScope handle_scope(m_pIsolate);
	v8::TryCatch try_catch(m_pIsolate);
	v8::Local<v8::Context> context = m_context.Get(m_pIsolate);
	v8::Context::Scope context_scope(context);

	v8::Local<v8::Value> result;
	v8::Local<v8::Script> v8Script = v8::Local<v8::Script>::New(m_pIsolate, m_script);
	if (!v8Script->Run(context).ToLocal(&result))
	{
		assert(try_catch.HasCaught());
		// Print errors that happened during execution.
		proccessException(&try_catch);
		return false;
	}
	else
	{
		// v8::MicrotasksScope::PerformCheckpoint(m_pIsolate);
		assert(!try_catch.HasCaught());
		if (!result->IsUndefined())
		{
			// If all went well and the result wasn't undefined then print
			// the returned value.
			v8::String::Utf8Value str(m_pIsolate, result);
			const char* cstr = *str;
			journal::Journal<journal::Severity::Fatal>() << "execute result: " << cstr;
		}
		return true;
	}

	// It is important not to pump the message loop when there are v8::Local
	// handles on the stack, as this may trigger a stackless GC.
	while (v8::platform::PumpMessageLoop(m_upPlatform.get(), m_pIsolate)) 
		continue;
}

void V8Engine::proccessException(v8::TryCatch* pTryCatch)
{
	v8::HandleScope handle_scope(m_pIsolate);
	v8::String::Utf8Value exception(m_pIsolate, pTryCatch->Exception());
	const char* exception_string = *exception;
	v8::Local<v8::Message> message = pTryCatch->Message();
	if (message.IsEmpty())
	{
		// V8 didn't provide any extra information about this error; just
		// print the exception.
		journal::Journal<journal::Severity::Fatal>() << exception_string;
	}
	else
	{
		// Print (filename):(line number): (message).
		v8::String::Utf8Value filename(m_pIsolate, message->GetScriptOrigin().ResourceName());
		v8::Local<v8::Context> context(m_pIsolate->GetCurrentContext());
		const char* filename_string = *filename;
		const int linenum = message->GetLineNumber(context).FromJust();
		journal::Journal<journal::Severity::Fatal>() << std::format("{}:{}: {}", filename_string, linenum, exception_string).c_str();
		// Print line of source code.
		v8::String::Utf8Value sourceline(m_pIsolate, message->GetSourceLine(context).ToLocalChecked());
		const char* sourceline_string = *sourceline;
		journal::Journal<journal::Severity::Fatal>() << sourceline_string;
		// Print wavy underline (GetUnderline is deprecated).
		int start = message->GetStartColumn(context).FromJust();
		auto journal = journal::Journal<journal::Severity::Fatal>();
		for (int i = 0; i < start; i++)
			journal << " ";
		int end = message->GetEndColumn(context).FromJust();
		for (int i = start; i < end; i++)
			journal << "^";
		v8::Local<v8::Value> stack_trace_string;
		if (pTryCatch->StackTrace(context).ToLocal(&stack_trace_string) &&
			stack_trace_string->IsString() &&
			stack_trace_string.As<v8::String>()->Length() > 0)
		{
			v8::String::Utf8Value stack_trace(m_pIsolate, stack_trace_string);
			const char* err = *stack_trace;
			journal::Journal<journal::Severity::Fatal>() << err;
		}
	}
}