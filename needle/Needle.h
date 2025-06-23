#pragma once

#include "FixedString.h"

namespace needle
{
	template<FixedString name, auto placeholder, typename C, typename... Args>
    struct Needle
    {
        virtual void visit() = 0;
    };

    template<FixedString name, auto fieldPtr, typename R, typename C>
    struct Needle<name, fieldPtr, R C::*>
    {
        virtual void visit() = 0;
    };

    template<FixedString name, auto methodPtr, typename R, typename C, typename... Args>
    struct Needle<name, methodPtr, R(C::*)(Args...)>
    {
        virtual void visit() = 0;
    };
}