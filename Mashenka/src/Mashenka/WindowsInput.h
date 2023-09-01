#pragma once
#include <map>

#include "Input.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Mashenka
{
    class MASHENKA_API WindowsInput : public Input
    {
    public:
        void PollImpl() override;
        bool IsKeyPressedImpl(int keycode) override;
        bool IsMouseButtonPressedImpl(int button) override;
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

