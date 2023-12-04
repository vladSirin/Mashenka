#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Platform/OpenGL/OpenGLShader.h"


Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) // 1280.0f / 720.0f is the aspect ratio
{
}

void Sandbox2D::OnAttach()
{
    Layer::OnAttach();

    // ==================== Create the Vertex Array ====================
    // Create the vertex array
    m_SquareVA = Mashenka::VertexArray::Create();

    // Create the vertex buffer
    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 0
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // 3
    };

    Mashenka::Ref<Mashenka::VertexBuffer> squareVB;
    squareVB.reset(Mashenka::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    Mashenka::BufferLayout layout = {
        { Mashenka::ShaderDataType::Float3, "a_Position" },
        { Mashenka::ShaderDataType::Float2, "a_TexCoord" }
    };
    squareVB->SetLayout(layout);
    m_SquareVA->AddVertexBuffer(squareVB); // Add the vertex buffer to the vertex array

    // Create the index buffer
    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Mashenka::Ref<Mashenka::IndexBuffer> squareIB;
    squareIB.reset(Mashenka::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    m_SquareVA->SetIndexBuffer(squareIB); // Add the index buffer to the vertex array

    // ==================== Create the Shader ====================
    m_FlatColorShader = Mashenka::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Mashenka::TimeStep ts)
{
    Layer::OnUpdate(ts);

    // Update
    m_CameraController.OnUpdate(ts);

    //render
    Mashenka::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Mashenka::RenderCommand::Clear();

    // Prepare the scene
    Mashenka::Renderer::BeginScene(m_CameraController.GetCamera()); // The camera is used to draw the scene

    // bind shader and set uniforms for color
    std::dynamic_pointer_cast<Mashenka::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Mashenka::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    // Draw the square
    Mashenka::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Mashenka::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    Layer::OnImGuiRender();

    // Setup a settings window and use Square Color as a color picker
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Mashenka::Event& e)
{
    Layer::OnEvent(e);
    m_CameraController.OnEvent(e); // The camera controller is used to handle events
}
