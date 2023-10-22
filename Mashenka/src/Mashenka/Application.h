#pragma once
#include "Core.h"
#include "Window.h"
#include "Mashenka/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"

/*
 * implement a form of the Singleton pattern for the Application class.
 * The Singleton pattern ensures that a class has only one instance and provides a global point of access to that instance.
 */
namespace Mashenka
{
    class MASHENKA_API Application
    {
    public:
        Application();

        // virtual destructor to make sure the derived class destructor is called
        // explain this: https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
        virtual ~Application() = default;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        

        //Using a static function to get the sole instance of the application
        inline Window& GetWindow() {return *m_Window;}
        inline static Application& Get() {return *s_Instance;}

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer; //adding ImGuiLayer variable for the application as it should be handled inside the engine

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
        std::unique_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;

        // Example data for blue square: VAO and shader
        std::shared_ptr<Shader> m_BlueShader;
        std::shared_ptr<VertexArray> m_SquareVA;

        // declare a static global single instance to access
        static Application* s_Instance;
    };

    // To be defined in Client
    Application* CreateApplication();
}
