#pragma once

#include "FixedString.h"

namespace needle
{
	// Type use
	template<FixedString name, typename C, auto placeholder = nullptr>
	struct Sewable
	{
		template<template<FixedString, typename, auto> typename... Vs>
		void accept()
		{
			(Vs<name, C, placeholder>().visit(), ...);
		}
	};

	// Member method use
	template<FixedString name, typename R, typename C, auto fieldPtr>
	struct Sewable<name, R C::*, fieldPtr>
	{
		template<template<FixedString, typename, auto> typename... Vs>
		void accept()
		{
			(Vs<name, R C::*, fieldPtr>().visit(), ...);
		}
	};

	// Member method use
	template<FixedString name, typename R, typename C, typename... Args, auto methodPtr>
	struct Sewable<name, R(C::*)(Args...), methodPtr>
	{
		template<template<FixedString, typename, auto> typename... Vs>
		void accept()
		{
			(Vs<name, R(C::*)(Args...), methodPtr>().visit(), ...);
		}
	};
}