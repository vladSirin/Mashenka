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

    Application::Application()
    {
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

        // Generates a unique identifier for a new VAO and stores it in 'm_VertexArray'
        // At this point the VAO is created but not active
        glGenVertexArrays(1, &m_VertexArray);
        // This line activate VAO meaning that all subsequence calls related to vertex arrays will be stored here
        glBindVertexArray(m_VertexArray);

        /*When you first bind a VAO using glBindVertexArray,
         *it becomes the active VAO. Any subsequent calls to glBindBuffer with GL_ARRAY_BUFFER or
         *GL_ELEMENT_ARRAY_BUFFER will associate the VBO or EBO with the currently active VAO.
         *This is how OpenGL knows to link these buffer objects with the VAO.*/

        /*When you later generate and bind a VBO, the VAO will keep track of that VBO as part of its state.
         *This is crucial because it allows you to switch between different sets of vertex data and
         *configurations by simply binding different VAOs.
         *The VAO encapsulates all the state needed to specify per-vertex attribute data,
         *including which VBOs to use and how to interpret the raw vertex data for each attribute.*/

        // Generate a unique identifier to a new Vertex Buffer Object(s) and stores it in the variable
        // the number of buffers created can be specify by argument 1
        glGenBuffers(1, &m_VertexBuffer);
        
        // binds a buffer object to a specified buffer binding target.
        // This function call makes the VBO represented 'm_VertexBuffer' the active array buffer
        // Any OpenGL function that deals with array buffer will affect this VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
        // GL_ARRAY_BUFFER is constant that represents the array buffer binding point in the OpenGL state machine

        // Example data for a simple triangle
        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };
        
        // uploading data to GPU for rendering, the usage here means it will be upload once and draw many times
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

        // Specify how OpenGL should interpret the vertex data
        glEnableVertexAttribArray(0);
        // stride is the offset between attributes, as here is a matrix of floats (3*3),
        // the offset between each line is 3 times the size of float
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        // Generate and bind a new EBO (index buffer object), which will hold the index data
        glGenBuffers(1, &m_IndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

        // Example data for indices of the triangle
        unsigned int indices[3] = {0, 1, 2};
        // populate the bound EBO with index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Create the Vertex and Fragment shaders
        std::string vertexSrc = R"(
            #version 330 core
            
            layout(location = 0) in vec3 a_Position;

            out vec3 v_Position;
            
            void main()
            {
                v_Position = a_Position;
                gl_Position = vec4(a_Position, 1.0);
            }
        )";

        // The fragment shader is responsible for determining the color output of the fragment.
        std::string fragmentSrc = R"(
            #version 330 core
            
            layout(location = 0) out vec4 color;

            in vec3 v_Position;
            
            void main()
            {
                color = vec4(v_Position * 0.5 + 0.5, 1.0);
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
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr); // drawing function

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
