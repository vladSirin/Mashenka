#include "mkpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Mashenka/Application.h"
#include "backends/imgui_impl_opengl3.h"

// TEMP
#include <GLFW/glfw3.h>

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

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonRleasedEvent));
        dispatcher.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(ImGuiLayer::OnWindowResizedEvent));
    }

    // definition on different events
    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_LEFT_SUPER];

        // Returns false we might need other layers to handle this too
        return false;
    }

    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = false;

        return false;
    }

    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        if (keycode > 0 && keycode < 0x1000)
            io.AddInputCharacter((unsigned short)keycode);

        return false;
    }

    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }

    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheelH += e.GetXOffset();
        io.MouseWheel += e.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnMouseButtonRleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        return false;
    }

    bool ImGuiLayer::OnWindowResizedEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
        glViewport(0, 0, e.GetWidth(), e.GetHeight());

        return false;
    }
}

