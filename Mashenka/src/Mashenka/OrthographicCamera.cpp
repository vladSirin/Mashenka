#include "mkpch.h"
#include "OrthographicCamera.h"

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
}
