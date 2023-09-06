#include "mkpch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "Mashenka/Application.h"
#include "backends/imgui_impl_opengl3.h"
#include <backends/imgui_impl_glfw.h>

#include "GLFW/glfw3.h"

namespace Mashenka
{
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
    {
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
        // call original class func
        Layer::OnAttach();

        // ImGui uses contexts to store data, essentially initialize imgui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Default color theme
        ImGui::StyleColorsDark();

        // Gets a reference to the "ImGuiIO" structure
        // IO data includes (display size, input events, time etc.)
        ImGuiIO& io = ImGui::GetIO();
        (void)io; // suppress any unused variable warnings.

        // Bitwise Or Assignment for flags
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Controllers
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        Application& app = Application::Get();
        auto window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410"); // Initialize the back end
        // backend is the bridge between ImGui high level commands and low level rendering, input
        // and platform or graphic APIs used.
    }

    void ImGuiLayer::OnDetach()
    {
        Layer::OnDetach();

        ImGui_ImplOpenGL3_Shutdown(); //Shutdown rendering backend
        ImGui_ImplGlfw_Shutdown(); //Shutdown input and window backend
        ImGui::DestroyContext(); // free up the ImGui context, which is used for state track
    }

    void ImGuiLayer::Begin()
    {
        // Prepare the rendering and input/window glfw for new frame
        ImGui_ImplOpenGL3_NewFrame(); // setup state, clear butters.
        ImGui_ImplGlfw_NewFrame(); // handle input events and update mouse etc.
        ImGui::NewFrame(); // signal a new frame is starting, allow usage of commands to create GUI elements
    }

    void ImGuiLayer::End()
    {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();

        // Setup the display size by making up a ImVec2 data which is tailored for ImGui
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // if multi viewport enabled
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            // in graphic programming, glfw contexts are container for resources and states for rendering
            // This includes texture, shaders and state settings, Each window has its own context
            // When working with multiple windows or rendering targets, you might need to switch
            GLFWwindow* backup_current_context = glfwGetCurrentContext();

            // As it needs to update multiple windows on OS level, thus it needs to be restored afterwards
            // This makes sure later inputs are processed in the original window user is on now.
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender()
    {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }


}
