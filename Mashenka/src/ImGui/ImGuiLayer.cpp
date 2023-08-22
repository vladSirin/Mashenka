#include "mkpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "GLFW/glfw3.h"
#include "Mashenka/Application.h"
#include "Platform/OpenGL/imgui_impl_opengl3.h"

namespace Mashenka
{
    Mashenka::ImGuiLayer::ImGuiLayer()
        :Layer("ImGuiLayer")
    {
    }

    Mashenka::ImGuiLayer::~ImGuiLayer()
    {
    }

    void Mashenka::ImGuiLayer::OnAttach()
    {
        // call original class func
        Layer::OnAttach();

        // ImGui uses contexts to store data, essentially initialize imgui
        ImGui::CreateContext();

        // Default color theme
        ImGui::StyleColorsDark();

        // Gets a reference to the "ImGuiIO" structure
        ImGuiIO& io = ImGui::GetIO();

        // Bitwise OR assignment
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // TEMPORARY: should eventually use Mashenka key codes
        io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
        io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
        io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
        io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
        io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

        // Init the opengl version 3
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void Mashenka::ImGuiLayer::OnDetach()
    {
        Layer::OnDetach();
    }

    void Mashenka::ImGuiLayer::OnUpdate()
    {
        Layer::OnUpdate();

        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();

        // Sets the display size in ImGuiIO to the application window
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        // Calculate delta time, if first frame which m_Time is 0, set to for 60 fps
        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f/60.0f);
        m_Time = time;

        // Signal the new frame is starting
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // show var to control the display of the demo window for debug
        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        // Generates the draw command
        ImGui::Render();
        // send to GPU to render
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Mashenka::ImGuiLayer::OnEvent(Event& event)
    {
        Layer::OnEvent(event);
    }
}

