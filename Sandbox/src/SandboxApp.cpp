#include "Mashenka.h"
#include "glm/gtc/type_ptr.hpp"

// Should not include anything else than the engine to make it work, below is TEMP


/*
 * 3. **Creating a Sandbox Class**:
 * A sandbox class is created as an instance of the Hazel application.
 * This class is defined in the client and returned by the `create application` function. 
 */

// Test layer
class ExampleLayer : public Mashenka::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
    {
        // ==================== OpenGL BELOW ====================
        // ==================== Prepare for Triangle Example ====================
        // Initialize the OpenGL function pointers
        m_VertexArray.reset(Mashenka::VertexArray::Create());

        // Example data for the triangle
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        // Create the vertex buffer
        Mashenka::Ref<Mashenka::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Mashenka::VertexBuffer::Create(vertices, sizeof(vertices)));

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
        Mashenka::Ref<Mashenka::IndexBuffer> indexBuffer;
        indexBuffer.reset(Mashenka::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        // Bind the index buffer to the vertex array
        m_VertexArray->SetIndexBuffer(indexBuffer);

        // ==================== Prepare for Square Example ====================
        // Create the vertex array for the square
        m_SquareVA.reset(Mashenka::VertexArray::Create());
        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
            0.75f, -0.75f, 0.0f,
            0.75f, 0.75f, 0.0f,
            -0.75f, 0.75f, 0.0f
        };

        Mashenka::Ref<Mashenka::VertexBuffer> squareVB;
        squareVB.reset(Mashenka::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            {Mashenka::ShaderDataType::Float3, "a_Position"}
        });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Mashenka::Ref<Mashenka::IndexBuffer> squareIB;
        squareIB.reset(Mashenka::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

        // new shader object with the vertex and fragment shader source code
        // using reset to reset the m_Shader pointer to the new shader object
        m_Shader.reset(Mashenka::Shader::Create(vertexSrc, fragmentSrc));

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

        m_FlatColorShader.reset(Mashenka::Shader::Create(FlatColorShaderVertexSrc, FlatColorShaderFragmentSrc));
    }

    void OnUpdate(Mashenka::TimeStep ts) override
    {
        // MK_INFO("ExampleLayer::Update");
        MK_TRACE("{0} update started", this->GetName());

        // Log the TimeStep
        MK_TRACE("Delta time: {0}s ({1}ms)", ts.GetSeconds(), ts.GetMilliseconds());

        // Moving the camera by arrow keys and rotate with Q and E
        if (Mashenka::Input::IsKeyPressed(MK_KEY_LEFT))
        {
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        }
        else if (Mashenka::Input::IsKeyPressed(MK_KEY_RIGHT))
        {
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;
        }
        if (Mashenka::Input::IsKeyPressed(MK_KEY_UP))
        {
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        }
        else if (Mashenka::Input::IsKeyPressed(MK_KEY_DOWN))
        {
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
        }

        // Rotate the camera
        if (Mashenka::Input::IsKeyPressed(MK_KEY_Q))
        {
            m_CameraRotation += m_CameraRotationSpeed;
        }
        else if (Mashenka::Input::IsKeyPressed(MK_KEY_E))
        {
            m_CameraRotation -= m_CameraRotationSpeed;
        }

        // set the camera position and rotation
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);


        // ==Render Pipeline==
        // call on render command
        Mashenka::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Mashenka::RenderCommand::Clear();

        // Prepare the scene and Bind the Shaders
        Mashenka::Renderer::BeginScene(m_Camera);
        std::dynamic_pointer_cast<Mashenka::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<Mashenka::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3(
            "u_Color", m_SquareColor);
        
        // prepare the transform matrix
        // Example: draw 10 boxes along a same line
        for (int i = 0; i < 10; ++i)
        {
            m_SquarePosition = {i * 0.16f, 0.0f, 0.0f};
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_SquarePosition) * glm::scale(
                glm::mat4(1.0f), glm::vec3(0.1f));
            Mashenka::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
        }


        m_Shader->Bind();
        Mashenka::Renderer::Submit(m_Shader, m_VertexArray);
        // End the scene
        Mashenka::Renderer::EndScene();
    }

    // ImGui Color Editor
    void OnImGuiRender() override
    {
        ImGui::Begin("Color Editor");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

    void OnEvent(Mashenka::Event& event) override
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
    }

private:
    /*m_VertexArray: This is the ID of the Vertex Array Object (VAO).
 *A VAO encapsulates all of the state needed to specify per-vertex attribute data to the OpenGL pipeline.
 *It essentially serves as a container for VBOs and EBOs.
 
m_VertexBuffer: This is the ID of the Vertex Buffer Object (VBO).
A VBO is a memory buffer in the high-speed memory of your video card designed to hold information about vertices.
In a 3D application, these could include coordinates, normals, and texture coordinates.

m_IndexBuffer: This is the ID of the Element Buffer Object (EBO), also known as the Index Buffer Object (IBO).
It stores the indices that OpenGL uses to decide which vertices to draw,
allowing for the reuse of vertex data and thus more efficient rendering.*/

    // Explanation: https://www.khronos.org/opengl/wiki/Shader_Compilation
    // The shader program is the final linked version of multiple shaders combined.
    // The shader program is the one that is used in the rendering process.
    // The shader program is the one that is stored in the OpenGL state machine.

    // Example data for the triangle: VAO and shader
    Mashenka::Ref<Mashenka::Shader> m_Shader;
    Mashenka::Ref<Mashenka::VertexArray> m_VertexArray;
    glm::vec3 m_TrianglePosition = {0.0f, 0.0f, 0.0f};

    // Example data for blue square: VAO and shader
    Mashenka::Ref<Mashenka::Shader> m_FlatColorShader;
    Mashenka::Ref<Mashenka::VertexArray> m_SquareVA;
    glm::vec3 m_SquarePosition = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_SquareColor = {0.2f, 0.3f, 0.8f};


    // Camera and properties
    // Why Define the camera here? Because the camera is a part of the layer, and the layer is a part of the application
    // The camera is a part of the layer because the camera is a part of the scene
    Mashenka::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_CameraRotation = 0.0f;
    float m_CameraTranslationSpeed = 0.1f;
    float m_CameraRotationSpeed = 0.1f;
};


/* ==
 * == SandBox App for the Mashenka Game Engine ====================
 */

// The Sandbox class is the client application that using the engine
class Sandbox : public Mashenka::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer);
    }

    // destructor
    ~Sandbox() = default;
};

Mashenka::Application* Mashenka::CreateApplication()
{
    printf("Sandbox is now on!");
    return new Sandbox();
}
