#include "mkpch.h"
#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/exterior_product.hpp"
#include "glm/gtx/matrix_cross_product.hpp"

namespace Mashenka
{
    void Camera::RecalculateViewMatrix()
    {
        // Recalculate the view matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }
}


