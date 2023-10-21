#include "mkpch.h"
#include "Application.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "Mashenka/Log.h"
#include <glad/glad.h>
#include "Input.h"
#include "Platform/Windows/WindowsWindow.h"

namespace Mashenka
{
    // initialize the singleton instance of application as null
    Application* Application::s_Instance = nullptr;

    // Define the static Create function
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case Mashenka::ShaderDataType::Float: 
        case Mashenka::ShaderDataType::Float2: 
        case Mashenka::ShaderDataType::Float3: 
        case Mashenka::ShaderDataType::Float4: 
        case Mashenka::ShaderDataType::Mat3: 
        case Mashenka::ShaderDataType::Mat4: return GL_FLOAT;
        case Mashenka::ShaderDataType::Int: 
        case Mashenka::ShaderDataType::Int2: 
        case Mashenka::ShaderDataType::Int3: 
        case Mashenka::ShaderDataType::Int4: 
        case Mashenka::ShaderDataType::Bool: return GL_BOOL;
        case Mashenka::ShaderDataType::None: break;
        }

        MK_CORE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }

    // this is the constructor of the application class
    Application::Application()
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


        // ==================== OpenGL ====================
        // Generates a unique identifier for a new VAO [Vertex Array Object] and stores it in 'm_VertexArray'
        // At this point the VAO is created but not active
        glGenVertexArrays(1, &m_VertexArray);
        // This line activate VAO meaning that all subsequence calls related to vertex arrays will be stored here
        glBindVertexArray(m_VertexArray);

        // Example data for the triangle
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        // Create the vertex buffer
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

        // Create the layout of the buffer, which is the layout of the vertex buffer
        {
            BufferLayout layout = {
                {ShaderDataType::Float3, "a_Position"},
                {ShaderDataType::Float4, "a_Color"}
            };

            m_VertexBuffer->SetLayout(layout);
        }

        // Bind the vertex buffer, which is the buffer that contains the vertices to be drawn by the GPU
        uint32_t index = 0;
        const auto& layout = m_VertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            // Enable the vertex attribute array
            glEnableVertexAttribArray(index);
            // Specify the location and data format of the array of generic vertex attributes at index
            glVertexAttribPointer(
                index,
                static_cast<GLint>(element.GetComponentCount()),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                static_cast<GLsizei>(layout.GetStride()),
                reinterpret_cast<const void*>(static_cast<uintptr_t>(element.Offset))  // NOLINT(performance-no-int-to-ptr)
            );
            index++;
        }


        // Example data for indices of the triangle
        unsigned int indices[3] = {0, 1, 2};
        // Create the index buffer, which is the buffer that contains the indices of the vertices to be drawn by the GPU
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        // Create the Vertex and Fragment shaders
        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;
            layout(location = 1) in vec4 a_Color;

            out vec3 v_Position;
            out vec4 v_Color;
            
            void main()
            {
                v_Position = a_Position;
                v_Color = a_Color;
                gl_Position = vec4(a_Position, 1.0);
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
    }

    Application::~Application()
    {
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

            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            // Bind the shader
            m_Shader->Bind();

            //The VAO contains pointers to the vertex buffers (VBOs) and their layout specifications,
            //as well as references to the index buffers (EBOs) if any.
            //By binding a VAO, you're effectively binding all these associated resources in one go,
            //making your code more efficient and easier to manage.
            glBindVertexArray(m_VertexArray); // bind Vertex Array Object
            glDrawElements(GL_TRIANGLES, static_cast<GLint>(m_IndexBuffer->GetCount()), GL_UNSIGNED_INT, nullptr); // drawing function

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
