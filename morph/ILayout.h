#pragma once

#include <array>
#include <string>

#include "../eventable/EventArgs.h"

namespace morph
{
    struct ILayout
    {
        using Color = std::array<unsigned char, 4>;

        virtual ~ILayout() {};

        virtual void setProperty(const std::string& property, const std::string& value) = 0;
        virtual double getProperty(const std::string& property) = 0;
        virtual void calculate(const double width, const double height) = 0;
        virtual void onLayoutChanged(const eventable::EventArgs& onAddedArgs) = 0;
    };
}