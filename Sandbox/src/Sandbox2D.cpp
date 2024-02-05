#include "Sandbox2D.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn> // This is a timer template for a function that takes in a lambda
class Timer
{
public:
    // The constructor takes in a name and a function, which is a lambda that takes in a ProfileResult and returns void
    Timer(const char* name, Fn&& func)
        : m_Name(name), m_Func(std::move(func)), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        m_Stopped = true;

        float duration = (end - start) * 0.001f;

        // print into console if duration is > 0.2ms
        if (duration > 0.2f)
        {
            // Print the name and time of the timer
            printf("%s: %fms\n", m_Name, duration);
        }
        
        // Call the function that was passed in, passing in the name and duration of the timer as a ProfileResult
        m_Func({ m_Name, duration });
    }

private:
    const char* m_Name;
    Fn m_Func;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
    bool m_Stopped;
};

// Example usage: PROFILE_FUNCTION(); or PROFILE_SCOPE("Function Name")
// This will create a timer and stop it when it goes out of scope, measuring the time it took to execute the code
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })




Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) // 1280.0f / 720.0f is the aspect ratio
{
}

void Sandbox2D::OnAttach()
{
    m_CheckerboardTexture = Mashenka::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
    Layer::OnDetach();
}

void Sandbox2D::OnUpdate(Mashenka::TimeStep ts)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate");
    Layer::OnUpdate(ts);

    // Update
    {
        PROFILE_SCOPE("CameraController::OnUpdate");
        m_CameraController.OnUpdate(ts);
    }

    //render
    {
        PROFILE_SCOPE("Render Prep");
        Mashenka::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Mashenka::RenderCommand::Clear();
    }


    // Begin scene
    {
        PROFILE_SCOPE("Render Draw");
        Mashenka::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Mashenka::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 1.5f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
        Mashenka::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 1.5f, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        Mashenka::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 1.0f, { 10.0f, 10.0f }, m_CheckerboardTexture);
        Mashenka::Renderer::EndScene();
    }

}

void Sandbox2D::OnImGuiRender()
{
    Layer::OnImGuiRender();

    // Setup a settings window and use Square Color as a color picker
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

    for (const auto& result : m_ProfileResults)
    {
        // Print the name and time of each ProfileResult
        char label[50];
        strcpy_s(label, "%.3fms "); // Copy the string "%.3fms " into label
        strcat_s(label, result.Name); // Concatenate result.Name onto label
        ImGui::Text(label, result.Time);
    }
    m_ProfileResults.clear(); // Clear the ProfileResults vector
    
    ImGui::End();
}

void Sandbox2D::OnEvent(Mashenka::Event& e)
{
    PROFILE_SCOPE("Sandbox2D::OnEvent");
    Layer::OnEvent(e);
    m_CameraController.OnEvent(e); // The camera controller is used to handle events
}
