#pragma once

namespace playable
{
    struct PlayableAspect
    {
        // Constructor
        template<typename T, typename... Args>
        void pre(Args&&... /*args*/) {}

        // Member  method
        template<typename R, typename T, typename... Args>
        void pre(R(T::* methodPtr)(Args...), Args&&... /*args*/)
        {
        }

        // Memeber method
        template<typename R, typename T, typename... Args>
        void post(R* pReturn, R(T::* methodPtr)(Args...), Args&&... /*args*/)
        {
        }
    };
}