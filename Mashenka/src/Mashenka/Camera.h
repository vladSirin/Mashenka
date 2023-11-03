#pragma once
#include "Events/Event.h"
#include "glm/glm.hpp"

namespace Mashenka
{

    // This is a camera base class for Mashenka Engine
    class Camera
    {
    public:
        // constructor & destructor
        Camera() = default;
        virtual ~Camera() = default;

        // camera frustum struct
        struct Frustum
        {
            float left, right, bottom, top, nearPlane, farPlane;
        };
        
        // getter & setter
        inline const glm::vec3& GetPosition() const {return m_Position;}
        inline void SetPosition(const glm::vec3& position) {m_Position = position; RecalculateViewMatrix();}
        inline float GetRotation() const {return m_Rotation;}
        inline void SetRotation(float rotation) {m_Rotation = rotation; RecalculateViewMatrix();}

        // Get the view matrix
        inline const glm::mat4& GetViewMatrix() const {return m_ViewMatrix;}
        // Get the projection matrix
        inline const glm::mat4& GetProjectionMatrix() const {return m_ProjectionMatrix;}
        // Get the view projection matrix
        // What is view projection matrix? It is the matrix that transform the world space to clip space
        inline const glm::mat4& GetViewProjectionMatrix() const {return m_ViewProjectionMatrix;}

        // On Event for camera
        virtual void OnEvent(Event& e) {}

    protected:
        void RecalculateViewMatrix();
        
        // Matrix
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        // Properties
        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        float m_Rotation = 0.0f;
        
    };
}
