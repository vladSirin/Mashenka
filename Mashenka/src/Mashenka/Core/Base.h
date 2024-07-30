#pragma once
#include <memory>
#include "Mashenka/Core/PlatformDetection.h"


/*
 * 2. **Setting up the Mashenka Log Macros**:
 * The Mashenka Log Macros are set up to log messages to the console.
 * This is achieved by defining macros that call the Mashenka Log functions.
 */

#ifdef MK_DEBUG
    #if defined(MK_PLATFORM_WINDOWS)
    #define MK_DEBUGBREAK() __debugbreak()
    #elif defined(MK_PLATFORM_LINUX)
    #include <signal.h>
    #define MK_DEBUGBREAK() raise(SIGTRAP)
    #else
    #error "Platform doesn't support debugbreak yet"
    #endif
    #define MK_ENABLE_ASSERTS
#else
    #defined MK_DEBUGBREAK()
#endif

#define MK_EXPAND_MACRO(x) x
#define MK_STRINGIFY_MACRO(x) #x

/*
 * 3. **Setting up Scope/Ref instead of raw pointers**:
 */
namespace Mashenka
{
    // Explain the following code snippet
    /*
     * The following code snippet is a template alias.
     * It's a way to create a new name for an existing type.
     * In this case, the type is std::unique_ptr<T>.
     * The new name is Scope<T>.
     */
    template <typename T>
    using Scope = std::unique_ptr<T>;

    // Template alias for creating a scope
    // Using Args&& ... args to forward the arguments so it supports variadic arguments
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...); //perfect forwarding for variadic arguments
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#include "Mashenka/Core/Log.h"
#include "Mashenka/Core/Assert.h"

/*
* The macro BIT(x) is defined as (1 << (x)). This is a bit shift operation.
Here's what it does:
1 is a binary number. In binary, 1 is represented as 0001 (for a 4-bit system for simplicity).
<< is the left shift operator. It shifts the bits of the number on the left x places to the left.
For example, if x is 2, then 1 << 2 would shift the bits of 1 two places to the left, resulting in 0100
(or 4 in decimal).
So, BIT(x) essentially creates a number that has its xth bit set (counting from 0).
This can be useful for creating bitmasks, which are often used in programming to
manipulate specific bits of a number.

For example, BIT(0) would be 1 (binary 0001), BIT(1) would be 2 (binary 0010),
BIT(2) would be 4 (binary 0100), and so on.
 */
#define BIT(x) (1 << (x))

/*The resulting callable object can be called with one argument,
 *and when it is called, it will invoke the fn member function of the parent object,
 *passing along the given argument.*/
// #define MK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

/* Lambda function
 * Capturing 'this' and packing all the parameters, using the return type of fn by -> decltype(auto)
 * Forwarding all the arguments to the member function fn by std::forward
 * decltype(x) is a function to runtime deduct the type, -> in lambda is to specify the return type
 * [] is for capturing, empty means it captures nothing, [this] it is capturing current object members
 */ 
#define MK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


