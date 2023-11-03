#pragma once
#include "Mashenka/Window.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include "Mashenka/Renderer/GraphicsContext.h"

namespace Mashenka
{
    // This is a sub class from windows (or a implementation on Windows platform)
    class WindowsWindow: public Window
    {
    public:

        // Constructor for WindowsWindow
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow() override;

        void OnUpdate() override;

        inline unsigned int GetWidth() const override {return m_Data.Width;}
        inline unsigned int GetHeight() const override {return m_Data.Height;}

        // Window Attributes
        // Set EventCallback function is just Set callback for m_Data, which is a WindowData struct
        // This is later used in application class so that the EventCallbackFn will be bind to the application::OnEvent
        inline void SetEventCallback(const EventCallbackFn& callback) override {m_Data.EventCallback = callback;}
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const override {return m_Window;}

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();
        
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;

        // WindowData is for holding the properties on runtime, while WindowProp is for creation/init
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
            
        };

        WindowData m_Data;
    };
}
