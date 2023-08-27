#pragma once


/*
 * 1. **Setting up the Mashenka API Macro**:
 * The Mashenka API Macro is set up to either export or import symbols based on
 * whether the Mashenka library is being built or used.
 * This is achieved by defining a macro that changes based on the build context. 
 */
#ifdef MK_PLATFORM_WINDOWS
    #ifdef MK_BUILD_DLL
        #define MASHENKA_API __declspec(dllexport)
    #else
        #define MASHENKA_API __declspec(dllimport)
    #endif
#else 
    #error Mashenka only support Windows!
#endif

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



