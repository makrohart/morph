#include <algorithm>
#include <iostream>

#include "aspectable/Aspectable.h"
#include "journal/Journal.h"
#include "needle/Needle.h"
#include "needle/Sewable.h"

template<needle::FixedString name, typename C, auto placeholder = nullptr>
struct ConcreteNeedle : needle::Needle<name, C>
{
    void visit() override
    {
        auto fn = []() {
            name;
            };
        fn();
    }
};

template<needle::FixedString name, typename R, typename C, auto fieldPtr>
struct ConcreteNeedle<name, R C::*, fieldPtr> : needle::Needle<name, R C::*, fieldPtr>
{
    void visit() override
    {
        auto fn = []() {
            name;
            };
        fn();
    }
};

template<needle::FixedString name, typename R, typename C, typename... Args, auto methoPtr>
struct ConcreteNeedle<name, R(C::*)(Args...), methoPtr> : needle::Needle<name, R(C::*)(Args...), methoPtr>
{
    void visit() override
    {
        auto fn = []() {
            name;
            };
        fn();
    }
};

struct A
{
    int run(int) { return 1; }

    int m_intField;
};

struct B : aspectable::Aspectable<aspectable::Aspect>
{
    B() : aspectable::Aspectable<aspectable::Aspect>(this) {}

    int run(int) { return 1; }

    int m_intField;
};

int main(int argc, const char* argv[])
{
    B b;
    b(&B::run, 10);
    needle::Sewable<"A", A>().accept<ConcreteNeedle>();
    needle::Sewable<"name", decltype(&A::run), &A::run>().accept<ConcreteNeedle>();
    needle::Sewable<"m_intField", decltype(&A::m_intField), &A::m_intField>().accept<ConcreteNeedle>();


    journal::Journal<journal::Severity::Info>()<< "Hello " << "able";
    journal::Journal<journal::Severity::Fatal>()<< "Hello " << "able";

    std::cout << "Hello, able!\n";
    return 0;
}
