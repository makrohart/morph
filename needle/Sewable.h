#pragma once

#include <concepts>

#include "FixedString.h"

namespace needle
{
	// Type use
 	template<FixedString name, auto placeholder, typename C, typename... Args>
    struct Sewable
    {
        template<template<FixedString, auto, typename, typename...> typename... Vs>
        void accept()
        {
            (Vs<name, placeholder, C, Args...>().visit(), ...);
        }
    };

	// Member field use
 	template<FixedString name, auto fieldPtr, typename R, typename C>
    struct Sewable<name, fieldPtr, R C::*>
    {
        template<template<FixedString, auto, typename, typename...> typename... Vs>
        void accept()
        {
            (Vs<name, fieldPtr, R C::*>().visit(), ...);
        }
    };

    // Member property use
 	template<FixedString name, auto accessor, typename R, typename C>
        requires GetterSetterPair<decltype(accessor)>
    struct Sewable<name, accessor, R C::*>
    {
        template<template<FixedString, auto, typename, typename...> typename... Vs>
        void accept()
        {
            (Vs<name, accessor, R C::*>().visit(), ...);
        }
    };

	// Member method use
	template<FixedString name, auto methodPtr, typename R, typename C, typename... Args>
    struct Sewable<name, methodPtr, R(C::*)(Args...)>
    {
        template<template<FixedString, auto, typename, typename...> typename... Vs>
        void accept()
        {
            (Vs<name, methodPtr, R(C::*)(Args...)>().visit(), ...);
        }
    };
}