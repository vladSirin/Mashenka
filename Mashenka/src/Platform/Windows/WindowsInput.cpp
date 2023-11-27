#include "mkpch.h"
#include "WindowsInput.h"
#include <GLFW/glfw3.h>
#include <map>

#include "Mashenka/Core/Application.h"
#include "Mashenka/Core/KeyCodes.h"
#include "Mashenka/Core/MouseButtonCodes.h"


namespace Mashenka
{
    // Create the s_Instance singleton for calling
    Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

    void Mashenka::WindowsInput::PollImpl()
    {
        auto m_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        // POll Keyboard state
        for (int key = MK_KEY_SPACE; key <= MK_KEY_LAST; ++key)
        {
            keyState[key] = glfwGetKey(m_window, key) == GLFW_PRESS;
        }

        // Poll mouse state
        for (int button = 0; button <= MK_MOUSE_BUTTON_LAST; ++button)
        {
            mouseState[button] = glfwGetMouseButton(m_window, button) == GLFW_PRESS;
        }

        // Poll Mouse positions
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        mouseX = static_cast<float>(x);
        mouseY = static_cast<float>(y);
    }

    bool Mashenka::WindowsInput::IsKeyPressedImpl(int keycode)
    {
        return keyState[keycode];
    }

    bool Mashenka::WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        return mouseState[button];
    }

    std::pair<float, float> Mashenka::WindowsInput::GetMousePositionImpl()
    {
        return {mouseX, mouseY};
    }

    float Mashenka::WindowsInput::GetMouseXImpl()
    {
        return mouseX;
    }

    float Mashenka::WindowsInput::GetMouseYImpl()
    {
        return mouseY;
    }
}


