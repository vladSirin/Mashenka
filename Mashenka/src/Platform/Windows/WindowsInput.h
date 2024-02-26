#pragma once
#include <map>

#include "Mashenka/Core/Input.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Mashenka
{
    class WindowsInput : public Input
    {
    public:
        void PollImpl() override;
        bool IsKeyPressedImpl(Key keycode) override;
        bool IsMouseButtonPressedImpl(Mouse button) override;
        std::pair<float, float> GetMousePositionImpl() override;
        float GetMouseXImpl() override;
        float GetMouseYImpl() override;

        ~WindowsInput() override = default;

    private:
        // all var are private
        std::map<int, bool> keyState;
        std::map<int, bool> mouseState;
        float mouseX = 0, mouseY = 0;
    };
}

