#include "mkpch.h"
#include "OrthographicCameraController.h"

#include "Mashenka/Core/Input.h"
#include "Mashenka/Core/KeyCodes.h"

namespace Mashenka
{
    Mashenka::OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
    : m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {
    }

    void Mashenka::OrthographicCameraController::OnUpdate(TimeStep ts)
    {
        // Camera controls, WASD
        if (Input::IsKeyPressed(MK_KEY_A))
        {
            // based on rotation in radians so the camera will move regardless of the rotation
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(MK_KEY_D))
        {
            // based on rotation in radians
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        // Camera controls, WASD
        if (Input::IsKeyPressed(MK_KEY_W))
        {
            // based on rotation in radians
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }
        else if (Input::IsKeyPressed(MK_KEY_S))
        {
            // based on rotation in radians
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * ts;
        }

        // Camera rotations
        if (m_Rotation)
        {
            if (Input::IsKeyPressed(MK_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            else if (Input::IsKeyPressed(MK_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;

            // Why? Because we don't want the camera rotation to be too big
            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }
        
        // Set the camera position
        m_Camera.SetPosition(m_CameraPosition);

        // Set the camera translation speed based on zoom level, as the camera will move faster when zoomed out
        m_CameraTranslationSpeed = m_ZoomLevel; 
    }

    void Mashenka::OrthographicCameraController::OnEvent(Event& e)
    {
        // Dispatching events
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool Mashenka::OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        // Zooming in and out
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool Mashenka::OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        // Resizing the camera
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
}

