#include "mkpch.h"
#include "Window.h"

#ifdef MK_PLATFORM_WINDOWS
#include "Platform/Windows/WindowsWindow.h"
#endif


namespace Mashenka
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
        #ifdef MK_PLATFORM_WINDOWS
                return CreateScope<WindowsWindow>(props);
        #else
                MK_CORE_ASSERT(false, "Unknow Platform");
                return nullptr;
        #endif
    }
}
