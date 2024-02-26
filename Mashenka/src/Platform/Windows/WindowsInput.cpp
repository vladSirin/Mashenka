#include "mkpch.h"
#include "Platform/Windows/WindowsInput.h"
#include <GLFW/glfw3.h>
#include <map>

#include "Mashenka/Core/Application.h"
#include "Mashenka/Core/KeyCodes.h"
#include "Mashenka/Core/MouseCode.h"


namespace Mashenka
{
    // Create the s_Instance singleton for calling
    Scope<Input> Input::s_Instance = CreateScope<WindowsInput>();

    void Mashenka::WindowsInput::PollImpl()
    {
        auto m_window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        // POll Keyboard state
        for (int key = static_cast<int>(MK_KEY_SPACE); key <= static_cast<int>(MK_KEY_LAST); ++key)
        {
            keyState[key] = glfwGetKey(m_window, key) == GLFW_PRESS;
        }

        // Poll mouse state
        for (int button = 0; button <= static_cast<int>(MK_MOUSE_BUTTON_LAST); ++button)
        {
            mouseState[button] = glfwGetMouseButton(m_window, button) == GLFW_PRESS;
        }

        // Poll Mouse positions
        double x, y;
        glfwGetCursorPos(m_window, &x, &y);
        mouseX = static_cast<float>(x);
        mouseY = static_cast<float>(y);
    }

    // Check if the key is pressed, if it is return true
    bool Mashenka::WindowsInput::IsKeyPressedImpl(Key keycode)
    {
        return keyState[static_cast<int>(keycode)]; // return true if the key is pressed, else false
        // using this because the keyState is a map, and the key is the index of the map
        // the map is being updated by the PollImpl function
    }

    bool Mashenka::WindowsInput::IsMouseButtonPressedImpl(Mouse button)
    {
        return mouseState[static_cast<int>(button)];
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


