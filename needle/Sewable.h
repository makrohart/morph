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

    // Member getter use
    template<FixedString name, auto getterPtr, typename R, typename C, typename T>
    struct Sewable<name, getterPtr, R(C::*)(), T>
    {
        template<template<FixedString, auto, typename, typename...> typename... Vs>
        void accept()
        {
            (Vs<name, getterPtr, R(C::*)(), T>().visit(), ...);
        }
    };

    // Member setter use
    template<FixedString name, auto setterPtr, typename R, typename C, typename T>
    struct Sewable<name, setterPtr, void(C::*)(R), T>
    {
        template<template<FixedString, auto, typename, typename...> typename... Vs>
        void accept()
        {
            (Vs<name, setterPtr, void(C::*)(R), T>().visit(), ...);
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