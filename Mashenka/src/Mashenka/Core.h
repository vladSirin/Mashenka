#pragma once

/*
 * 1. **Setting up the Hazel API Macro**:
 * The Hazel API Macro is set up to either export or import symbols based on
 * whether the Hazel library is being built or used.
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