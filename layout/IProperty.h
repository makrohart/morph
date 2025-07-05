#pragma once

namespace layout
{
    struct IProperty
    {
        virtual ~IProperty() = 0;
    };

    struct DoubleProperty : IProperty
    {
        ~DoubleProperty() override = default;

        
    };

}