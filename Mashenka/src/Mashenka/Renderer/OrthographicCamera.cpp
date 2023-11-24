#include "mkpch.h"
#include "OrthographicCamera.h"
#include "glm/ext/matrix_clip_space.hpp"

namespace Mashenka
{
    // Constructor
    OrthographicCamera::OrthographicCamera(float orthoLeft, float orthoRight, float orthoBottom, float orthoTop)
        : Camera()
    {

        // Set view matrix
        m_ViewMatrix = glm::mat4(1.0f);
        // Set projection Matrix
        m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, -1.0f, 1.0f);
        // Set view projection matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

    void OrthographicCamera::OnEvent(Event& e)
    {
        Camera::OnEvent(e); 
        EventDispatcher dispatcher(e);
        
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        // Set projection Matrix
        m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        // Set view projection matrix
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}
