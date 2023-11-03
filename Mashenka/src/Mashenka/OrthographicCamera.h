#pragma once
#include "Camera.h"
#include "Events/KeyEvent.h"

namespace Mashenka
{
    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float orthoLeft, float orthoRight, float orthoBottom, float orthoTop);

        void OnEvent(Event& e) override;

    private:
        // Key pressed event
        bool OnKeyPressed(KeyPressedEvent& e);
  
    };
}
