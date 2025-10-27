#pragma once

#include <array>

namespace eventable
{
    struct EventArgs
    {
        EventArgs() {}
        EventArgs(std::array<double, 2> position) : position(position) {}

        virtual ~EventArgs() = default;

        std::array<double, 2> position;
    };
}