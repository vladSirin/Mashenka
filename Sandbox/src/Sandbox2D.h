#pragma once
#include "Mashenka.h"

// This is the Sandbox2D class
class Sandbox2D : public Mashenka::Layer
{
public:
    Sandbox2D();
    // The destructor is virtual because this class is inherited from Layer
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Mashenka::TimeStep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Mashenka::Event& e) override;

private:
    // Camera used for the 2D scene
    Mashenka::OrthographicCameraController m_CameraController;

    // Temp test
    Mashenka::Ref<Mashenka::VertexArray> m_SquareVA;
    Mashenka::Ref<Mashenka::Shader> m_FlatColorShader;

    glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
