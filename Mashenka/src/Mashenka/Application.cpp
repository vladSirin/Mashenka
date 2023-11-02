#include "mkpch.h"
#include "Application.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "Mashenka/Log.h"
#include <glad/glad.h>
#include "Input.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace Mashenka
{
    // This is the definition of the BIND_EVENT_FN function
    
    // initialize the singleton instance of application as null
    Application* Application::s_Instance = nullptr;

    // this is the constructor of the application class
    Application::Application()
        : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f) // Initialize the camera
    {
        // ==================== Make sure Singleton and setup Window ====================
        // Make sure there is only one instance
        MK_CORE_ASSERT(!s_Instance, "Application Alreay Exists!")
        s_Instance = this;

        // Create the window
        m_Window = std::unique_ptr<Window>(WindowsWindow::Create());


        // using bind to call the member function of application when needed in Window layer
        // set the application OnEvent function to the window layer as a callback
        m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

        // Create and assign the ImGuiLayer, Push it into the stack
        // This was done by sandbox app, which is not ideal as it should be part of the engine app
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);


        // ==================== OpenGL BELOW ====================
        // ==================== Prepare for Triangle Example ====================
        // Initialize the OpenGL function pointers
        m_VertexArray.reset(VertexArray::Create());

        // Example data for the triangle
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        // Create the vertex buffer
        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        // Create the layout of the buffer, which is the layout of the vertex buffer then bind it to the vertex array
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"}
        };
        vertexBuffer->SetLayout(layout);
        m_VertexArray->AddVertexBuffer(vertexBuffer);
        
        // Example data for indices of the triangle
        uint32_t indices[3] = {0, 1, 2};
        // Create the index buffer, which is the buffer that contains the indices of the vertices to be drawn by the GPU
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        // Bind the index buffer to the vertex array
        m_VertexArray->SetIndexBuffer(indexBuffer);

        // ==================== Prepare for Square Example ====================
        // Create the vertex array for the square
        m_SquareVA.reset(VertexArray::Create());
        float squareVertices[3 * 4] = {
            -0.75f, -0.75f, 0.0f,
             0.75f, -0.75f, 0.0f,
             0.75f,  0.75f, 0.0f,
            -0.75f, 0.75f, 0.0f
        };

        std::shared_ptr<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
        squareVB->SetLayout({
            {ShaderDataType::Float3, "a_Position"}
        });
        m_SquareVA->AddVertexBuffer(squareVB);

        uint32_t squareIndices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        m_SquareVA->SetIndexBuffer(squareIB);

        // ==================== Prepare for Shaders of Triangle and Square ====================
        // Create the Vertex and Fragment shaders
        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            out vec4 v_Color;
            
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0) * u_ViewProjection;
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
        m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

        // Create the Vertex and Fragment shaders for the blue square
        std:: string blueShaderVertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            uniform mat4 u_ViewProjection;

            out vec3 v_Position;
            
            void main()
            {
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0) * u_ViewProjection;
            }
        )";

        std::string blueShaderFragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            
            void main()
            {
                color = vec4(0.2, 0.3, 0.8, 1.0);
            }
        )";

        m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
    }

    // Define the OnEvent function, this works similar to all OnEvent functions
    // Basically it creates a dispatcher and bind the function to it
    void Application::OnEvent(Event& e)
    {
        // Create a dispatcher
        EventDispatcher dispatcher(e);

        // Dispatch (which takes a template func as input), will check if the input matches the type
        // If so, the bound event will be called (which is the Application.OnWindowClose functions)
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

        /*
         * The purpose of this loop seems to be to process an event e in reverse order of the layers in the m_LayerStack
         * until one of the layers handles the event. Once the event is handled, the loop stops,
         * ensuring that layers earlier in the stack don't process an already-handled event.
         * This is a common pattern in event-driven systems
         * where events can be consumed by layers or components in a specific order of priority.
         */
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }

        MK_CORE_TRACE("{0}", e);
    }

    // The on attach now is called in LayerStack when Push, which it should be.
    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
    }


    /*
     * 4. **Running the Application**:
     * The application is run by calling the `run` function on the application instance.
     * This initiates a while loop that keeps the application running.
     */
    void Application::Run()
    {
        while (m_Running)
        {
            // Poll Input
            Input::Poll();

            // ==Render Pipeline==
            // call on render command
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            RenderCommand::Clear();

            // Set the camera position and rotation
            m_Camera.SetPosition({0.5f, 0.5f, 0.0f});
            m_Camera.SetRotation(60.0f);
            // Prepare the scene and Bind the Shaders
            Renderer::BeginScene(m_Camera);
            m_BlueShader->Bind();
            Renderer::Submit(m_BlueShader, m_SquareVA);
            m_Shader->Bind();
            Renderer::Submit(m_Shader, m_VertexArray);
            // End the scene
            Renderer::EndScene();
            

            // Go through all the layers, as each layer can handle its own update
            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            // Initialize the ImGui frame, prepare for the rendering, context and input
            m_ImGuiLayer->Begin();

            // Go through all the layers, as each layer can handle its own ImGui component
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender(); // Render the needed info

            // finalize the rendering for the current frame, wraps up tasks like draw data
            m_ImGuiLayer->End();

            m_Window->OnUpdate();
        }
    }

    // Definition of the OnWindowClose function
    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
