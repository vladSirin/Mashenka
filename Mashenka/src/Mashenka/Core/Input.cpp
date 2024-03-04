#include "mkpch.h"
#include "Mashenka/Core/Input.h"

#ifdef MK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsInput.h"
#endif


namespace Mashenka
{
    // Create the s_Instance singleton for calling
    // This is called when the application is created, as in C++ the static variable is initialized
    // before the main function
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
        #ifdef MK_PLATFORM_WINDOWS
                return CreateScope<WindowsInput>();
        #else
                MK_CORE_ASSERT(false, "Unknow Platform");
                return nullptr;
        #endif
    }
}
