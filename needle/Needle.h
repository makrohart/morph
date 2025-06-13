#pragma once

#include "FixedString.h"

namespace needle
{
	template<FixedString name, typename C, auto placeholder = nullptr>
	struct Needle
	{
		virtual void visit() = 0;
	};

	template<FixedString name, typename R, typename C, auto fieldPtr>
	struct Needle<name, R C::*, fieldPtr>
	{
		virtual void visit() = 0;
	};

	template<FixedString name, typename R, typename C, typename... Args, auto methodPtr>
	struct Needle<name, R(C::*)(Args...), methodPtr>
	{
		virtual void visit() = 0;
	};
}