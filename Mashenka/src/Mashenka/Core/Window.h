#pragma once

#include <sstream>

#include "Mashenka/Core/Base.h"
#include "Mashenka/Events/Event.h"

namespace Mashenka
{
    // This is the window property struct, easier to manage, expand for future use.
    struct WindowProps
    {
        std::string Title;
        uint32_t Width;
        uint32_t Height;

        WindowProps(const std::string& title = "Mashenka Engine",
            uint32_t width = 1280,
            uint32_t height = 720)
                :Title(title), Width(width), Height(height)
        {
            
        }
    };

    // Interface representing a desktop system based Window
    class Window
    {
    public:
        // wrapping a callable object/method with Event& input
        using EventCallbackFn = std::function<void(Event&)>;

        // Window base functions, in the actual implementation will be platform specific
        /*= 0: This syntax makes the function a pure virtual function,
         *meaning that it has no implementation in the base class and must be overridden
         *in any non-abstract derived class.
         *A class with one or more pure virtual functions is considered an abstract class,
         *and you cannot create instances of it directly.*/
        virtual ~Window() = default;
        virtual void OnUpdate() = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        // Window Attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;
        virtual void* GetNativeWindow() const = 0;

        static Scope<Window> Create(const WindowProps& props = WindowProps());
    };
    
}
