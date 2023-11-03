#include "mkpch.h"
#include "OrthographicCamera.h"


#include "KeyCodes.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Mashenka
{
    // Constructor
    OrthographicCamera::OrthographicCamera(float orthoLeft, float orthoRight, float orthoBottom, float orthoTop)
    {
        // Set projection Matrix
        m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, -1.0f, 1.0f);
        // Set view projection matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::OnEvent(Event& e)
    {
        Camera::OnEvent(e);
        EventDispatcher dispatcher(e);

        // bind the dispatcher to a camera move function
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(OrthographicCamera::OnKeyPressed));
        
    }

    bool OrthographicCamera::OnKeyPressed(KeyPressedEvent& e)
    {
        // If the key is W, A, S, D, then move the camera
        if (e.GetKeyCode() == MK_KEY_W)
        {
            m_Position.y += 0.1f;
            RecalculateViewMatrix();
            // log
            MK_CORE_TRACE("Camera Position: {0}, {1}, {2}", m_Position.x, m_Position.y, m_Position.z);
            return true;
        }
        else if (e.GetKeyCode() == MK_KEY_A)
        {
            m_Position.x -= 0.1f;
            RecalculateViewMatrix();
            MK_CORE_TRACE("Camera Position: {0}, {1}, {2}", m_Position.x, m_Position.y, m_Position.z);
            return true;
        }
        else if (e.GetKeyCode() == MK_KEY_S)
        {
            m_Position.y -= 0.1f;
            RecalculateViewMatrix();
            MK_CORE_TRACE("Camera Position: {0}, {1}, {2}", m_Position.x, m_Position.y, m_Position.z);
            return true;
        }
        else if (e.GetKeyCode() == MK_KEY_D)
        {
            m_Position.x += 0.1f;
            RecalculateViewMatrix();
            MK_CORE_TRACE("Camera Position: {0}, {1}, {2}", m_Position.x, m_Position.y, m_Position.z);
            return true;
        }
        return false;
    }
}
