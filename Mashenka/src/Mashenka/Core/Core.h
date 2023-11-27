#pragma once
#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
    /* Windows x64/x86 */
    #ifdef _WIN64
        /* Windows x64  */
        #define MK_PLATFORM_WINDOWS
    #else
        /* Windows x86 */
        #error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    /* TARGET_OS_MAC exists on all the platforms
    * so we must check all of them (in this order)
    * to ensure that we're running on MAC
    * and not some other Apple platform */
    #if TARGET_IPHONE_SIMULATOR == 1
        #error "IOS simulator is not supported!"
    #elif TARGET_OS_IPHONE == 1
        #define MK_PLATFORM_IOS
        #error "IOS is not supported!"
    #elif TARGET_OS_MAC == 1
        #define MK_PLATFORM_MACOS
        #error "MacOS is not supported!"
    #else
        #error "Unknown Apple platform!"
    #endif
/* We also have to check __ANDROID__ before __linux__
 * since android is based on the linux kernel
 * it has __linux__ defined */
#elif defined(__ANDROID__)
    #define MK_PLATFORM_ANDROID
    #error "Android is not supported!"
#elif defined(__linux__)
    #define MK_PLATFORM_LINUX
    #error "Linux is not supported!"
#else
    /* Unknown compiler/platform */
    #error "Unknown platform!"
#endif // End of platform detection

// DLL support
#ifdef MK_PLATFORM_WINDOWS
    #ifdef MK_BUILD_DLL
        #define MASHENKA_API __declspec(dllexport)
    #else
        #define MASHENKA_API __declspec(dllimport)
    #endif
#else
    #define MASHENKA_API
#endif // End of DLL support

/*
 * 2. **Setting up the Mashenka Log Macros**:
 * The Mashenka Log Macros are set up to log messages to the console.
 * This is achieved by defining macros that call the Mashenka Log functions.
 */

#ifdef MK_DEBUG
    #define MK_ENABLE_ASSERTS
#endif

#ifdef MK_ENABLE_ASSERTS
    #define MK_ASSERT(x, ...) {if (!(x)) {MK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
    #define MK_CORE_ASSERT(x, ...) {if(!(x)) {MK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
#define MK_ASSERT(x, ...)
#define MK_CORE_ASSERT(x, ...)
#endif

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
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...); //perfect forwarding for variadic arguments
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}


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
#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)


