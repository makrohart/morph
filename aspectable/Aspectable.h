#pragma once

#include <memory>

namespace aspectable
{
    template<typename... As>
    struct Aspectable : As...
    {
        virtual ~Aspectable() = default;
        
        // Constructor
        template<typename T, typename... Args>  
        explicit Aspectable(T* pSelf, Args&&... args)
        {
            (..., As::pre<T>(std::forward<Args>(args)...));
        }   

        // TODO: function const is not supported yet;
        template<typename R, typename T, typename... Args>
        auto operator()(R (T::* methodPtr)(Args...), Args&&... args)
        {
            (..., As::pre(methodPtr, std::forward<Args>(args)...));

            if constexpr (std::is_void_v<R>) {
                (dynamic_cast<T*>(this)->*methodPtr)(std::forward<Args>(args)...);
                (..., As::post((R*)nullptr, methodPtr, std::forward<Args>(args)...));
            }
            else {
                R retVal = (dynamic_cast<T*>(this)->*methodPtr)(std::forward<Args>(args)...);
                (..., As::post(&retVal, methodPtr, std::forward<Args>(args)...));
                return retVal;
            }
        }
    };

    struct Aspect
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