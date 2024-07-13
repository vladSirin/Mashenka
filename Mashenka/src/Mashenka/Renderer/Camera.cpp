#include "mkpch.h"
#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/exterior_product.hpp"
#include "glm/gtx/matrix_cross_product.hpp"

namespace Mashenka
{
    void Camera::RecalculateViewMatrix()
    {
        MK_PROFILE_FUNCTION(); // Profiling
        // Recalculate the view matrix
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
            glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

        // ViewMatrix is the inverse of its transform as it is used to turn world transform into camera transform
        // Which means change the origin from world space to the camera as origin.
        // If we consider a point P in world space, C as camera transform: P<camera> = C^-1 * P<world>
        m_ViewMatrix = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; // this is to change to screen space

#ifdef MAT_EXAMPLE
        {
            glm::mat4 m_ModelMatrix;         // Model transformation
            glm::mat4 m_ViewMatrix;          // View transformation (inverse of camera transform)
            glm::mat4 m_ProjectionMatrix;    // Projection transformation

            // Combine view and projection matrices
            glm::mat4 m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

            // Final transformation from model to screen space
            glm::mat4 m_ModelViewProjectionMatrix = m_ViewProjectionMatrix * m_ModelMatrix;

            // Transform a vertex position
            glm::vec4 vertex_world = m_ModelMatrix * vertex_object;
            glm::vec4 vertex_clip = m_ViewProjectionMatrix * vertex_world;
        }
#endif

    }
}


