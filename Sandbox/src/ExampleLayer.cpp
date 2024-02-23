#include "ExampleLayer.h"

// Graphics and Imgui
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui/imgui.h"
#include "Platform/OpenGL/OpenGLShader.h"

// Test layer
ExampleLayer::ExampleLayer()
    : Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
{
    // ==================== OpenGL BELOW ====================
    // ==================== Prepare for Triangle Example ====================
    // Initialize the OpenGL function pointers
    m_VertexArray = Mashenka::VertexArray::Create();

    // Example data for the triangle
    float vertices[3 * 7] = {
        -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
        0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
        0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
    };

    // Create the vertex buffer
    Mashenka::Ref<Mashenka::VertexBuffer> vertexBuffer = Mashenka::VertexBuffer::Create(vertices, sizeof(vertices));

    // Create the layout of the buffer, which is the layout of the vertex buffer then bind it to the vertex array
    Mashenka::BufferLayout layout = {
        {Mashenka::ShaderDataType::Float3, "a_Position"},
        {Mashenka::ShaderDataType::Float4, "a_Color"}
    };
    vertexBuffer->SetLayout(layout);
    m_VertexArray->AddVertexBuffer(vertexBuffer);

    // Example data for indices of the triangle
    uint32_t indices[3] = {0, 1, 2};
    // Create the index buffer, which is the buffer that contains the indices of the vertices to be drawn by the GPU
    Mashenka::Ref<Mashenka::IndexBuffer> indexBuffer = Mashenka::IndexBuffer::Create(
        indices, sizeof(indices) / sizeof(uint32_t));
    // Bind the index buffer to the vertex array
    m_VertexArray->SetIndexBuffer(indexBuffer);

    // ==================== Prepare for Square Example ====================
    // Create the vertex array for the square
    m_SquareVA = Mashenka::VertexArray::Create();
    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    Mashenka::Ref<Mashenka::VertexBuffer> squareVB = Mashenka::VertexBuffer::Create(
        squareVertices, sizeof(squareVertices));
    squareVB->SetLayout({
        {Mashenka::ShaderDataType::Float3, "a_Position"},
        {Mashenka::ShaderDataType::Float2, "a_TexCoord"}
    });
    m_SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
    Mashenka::Ref<Mashenka::IndexBuffer> squareIB = Mashenka::IndexBuffer::Create(
        squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVA->SetIndexBuffer(squareIB);

    // ==================== Prepare for Shaders of Triangle and Square ====================
    // Create the Vertex and Fragment shaders
    std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            out vec4 v_Color;
            
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    // The fragment shader is responsible for determining the color output of the fragment.
    std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            in vec4 v_Color;
            
            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
                color = v_Color;
            }
        )";

    // Create the shader program, which is the final linked version of multiple shaders combined.
    // Why: to encapsulate all of the state needed to specify per-vertex attribute data to the OpenGL pipeline.
    // How: using a vertex array object (VAO), which essentially serves as a container for VBOs and EBOs.
    //     using a vertex buffer object (VBO), which is a memory buffer in the high-speed memory of your video card designed to hold information about vertices.
    m_Shader = Mashenka::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

    // Create the Vertex and Fragment shaders for the blue square
    std::string FlatColorShaderVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;

            out vec3 v_Position;
            
            void main()
            {
                v_Position = a_Position;
                gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
            }
        )";

    std::string FlatColorShaderFragmentSrc = R"(
            #version 330 core

            layout(location = 0) out vec4 Color;
            in vec3 v_Position;
            uniform vec3 u_Color;
            
            void main()
            {
                Color = vec4(u_Color, 0.5);
            }
        )";

    m_FlatColorShader = Mashenka::Shader::Create("FlatColor", FlatColorShaderVertexSrc, FlatColorShaderFragmentSrc);

    // ==================== Prepare for Texture ====================
    // Texture shader src
    auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
    m_Texture = Mashenka::Texture2D::Create("assets/textures/Checkerboard.png");
    m_ChernoLogoTexture = Mashenka::Texture2D::Create("assets/textures/ChernoLogo.png");
    std::dynamic_pointer_cast<Mashenka::OpenGLShader>(textureShader)->Bind();
    std::dynamic_pointer_cast<Mashenka::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
    Layer::OnAttach();
}

void ExampleLayer::OnDetach()
{
    Layer::OnDetach();
}

void ExampleLayer::OnUpdate(Mashenka::TimeStep ts)
{
    // Update the camera controller
    m_CameraController.OnUpdate(ts);


    // ==Render Pipeline==
    // call on render command
    Mashenka::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    Mashenka::RenderCommand::Clear();

    // Prepare the scene and Bind the Shaders
    Mashenka::Renderer::BeginScene(m_CameraController.GetCamera());
    std::dynamic_pointer_cast<Mashenka::OpenGLShader>(m_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Mashenka::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3(
        "u_Color", m_SquareColor);

    // prepare the transform matrix
    // Example: draw 10 boxes along a same line
    for (int i = 0; i < 10; ++i)
    {
        m_SquarePosition = {static_cast<float>(i) * 0.16f, 0.0f, 0.0f};
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition) * glm::scale(
            glm::mat4(1.0f), glm::vec3(0.1f));
        Mashenka::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
    }

    // Getting texture from the lib
    auto textureShader = m_ShaderLibrary.Get("Texture");
    // Draw the texture
    m_Texture->Bind(0);
    Mashenka::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    // Draw the logo
    m_ChernoLogoTexture->Bind(0);
    Mashenka::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));


    // Mashenka::Renderer::Submit(m_Shader, m_VertexArray);
    // End the scene
    Mashenka::Renderer::EndScene();
}

// ImGui Color Editor
void ExampleLayer::OnImGuiRender()
{
    ImGui::Begin("Color Editor");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}

void ExampleLayer::OnEvent(Mashenka::Event& event)
{
    // MK_TRACE("{0}", event);
    if (event.GetEventType() == Mashenka::EventType::KeyPressed)
    {
        const auto& e = dynamic_cast<Mashenka::KeyPressedEvent&>(event);
        if (e.GetKeyCode() == MK_KEY_TAB)
        {
            MK_TRACE("Tab Key is Pressed (event)!");
        }
        MK_TRACE("{0}", static_cast<char>(e.GetKeyCode()));
    }

    // camera event
    m_CameraController.OnEvent(event);
}
