#include <algorithm>
#include <iostream>

#include "aspectable/Aspectable.h"
#include "journal/Journal.h"
#include "jsEngine/JSEngine.h"
#include "jsEngine/V8Binding.h"
#include "jsEngine/V8Converter.h"
#include "jsEngine/V8Engine.h"
#include "needle/Sewable.h"

struct A : public aspectable::Aspectable<aspectable::Aspect>
{
    A() : aspectable::Aspectable<aspectable::Aspect>(this) {}

    int getIntField() { return m_intField; }
    void setIntField(const int value) { m_intField = value; }

    int run(int) 
    { 
        journal::Journal<journal::Severity::Info>() << "A::run";
        return 1; 
    }

    int m_intField;
};

int main(int argc, const char* argv[])
{
    needle::Sewable<"A", nullptr, A>().accept<V8Binding>();
    needle::Sewable<"run", &A::run, decltype(&A::run)>().accept<V8Binding>();
    needle::Sewable<"m_intField", &A::m_intField, decltype(&A::m_intField)>().accept<V8Binding>();
    needle::Sewable<"IntField", std::pair(&A::getIntField, &A::setIntField), decltype(&A::m_intField)>().accept<V8Binding>();

    journal::Journal<journal::Severity::Info>()<< "Hello " << "able";
    journal::Journal<journal::Severity::Fatal>()<< "Hello " << "able";

    JSEngine* engine = new V8Engine();
    engine->run({
        "D:/Projects/able/reacts-umd/dist/reacts.umd.js",
        "D:/Projects/able/out/build/x64-debug/Debug/Journal.js",
    });
    std::cout << "Hello, able!\n";
    return 0;
}
