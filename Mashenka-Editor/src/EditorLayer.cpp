﻿#include "EditorLayer.h"

#include <imgui/imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mashenka
{
    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f), m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f})
    {
    }

    void EditorLayer::OnAttach()
    {
        MK_PROFILE_FUNCTION();
        Layer::OnAttach();

        m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");

        FramebufferSpecification fbSpec;
        fbSpec.Width = 1280;
        fbSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(fbSpec);
    }

    void EditorLayer::OnDetach()
    {
        MK_PROFILE_FUNCTION();
        Layer::OnDetach();
    }

    void EditorLayer::OnUpdate(TimeStep ts)
    {
        MK_PROFILE_FUNCTION();
        Layer::OnUpdate(ts);

        //update
        m_CameraController.OnUpdate(ts);

        //render
        Renderer2D::ResetStats();
        {
            MK_PROFILE_SCOPE("Renderer Prep");
            m_Framebuffer->Bind();
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            RenderCommand::Clear();
        }

        {
            static float rotation = 0.0f;
            rotation += ts * 50.0f;

            MK_PROFILE_SCOPE("Renderer Draw");
            Renderer2D::BeginScene(m_CameraController.GetCamera());
            Renderer2D::DrawRotatedQuad({1.0f, 0.0f}, {0.8f, 0.8f}, -45.0f, {0.8f, 0.2f, 0.3f, 1.0f});
            Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
            Renderer2D::DrawQuad({0.5f, 0.5f}, {0.5f, 0.75f}, m_SquareColor);
            Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {20.0f, 20.0f}, m_CheckerboardTexture, 10.0f);
            Renderer2D::DrawRotatedQuad({-2.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, rotation, m_CheckerboardTexture, 20.0f);
            Renderer2D::EndScene();

            Renderer2D::BeginScene(m_CameraController.GetCamera());
            for (float y = -5.0f; y < 5.0f; y += 0.5f)
            {
                for (float x = -5.0f; x < 5.0f; x += 0.5f)
                {
                    glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f};
                    Renderer2D::DrawQuad({x, y}, {0.45f, 0.45f}, color);
                }
            }
            Renderer2D::EndScene();
            m_Framebuffer->Unbind();
        }
    }

    void EditorLayer::OnImGuiRender()
    {
        MK_PROFILE_FUNCTION();
        //Layer::OnImGuiRender();

        // Note: switch this to true to enable dockspace
        static bool dockingEnabled = true;
        if (dockingEnabled)
        {
            /* Static var manages the states
             * @dockspaceOpen control if the dockspace window is open
             * @opt_fullscreen_persistent persists the full screen state across frames
             * @opt_fullscreen to toggle fullscreen mode for the frame
             * @dockspace_flags to customize the behavior
             */
            static bool dockspaceOpen = true;
            static bool opt_fullscreen_persistent = true;
            bool opt_fullscreen = opt_fullscreen_persistent;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // we are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others
            ImGuiDockNodeFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            // Bitwise OR operation

            // setup params when the docking is fullscreen
            if (opt_fullscreen)
            {
                // Since it is fullscreen, getting the viewport pos and size to set on the window
                ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->Pos);
                ImGui::SetNextWindowSize(viewport->Size);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // get rid of window rounding
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // remove border size
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                    ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }

            // when using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
            // pass-thru hole, so we ask Begin() to not render a background
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) // Bitwise AND operation for check flags
            {
                window_flags |= ImGuiWindowFlags_NoBackground;
            }

            // Important: note that we proceed even if Begin() returns false (aks window is collapsed)
            // This is because we want to keep your dockspace() active, if a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible
            // PushStyleVar will change certain style for specific scope, the style will be reset when Pop is called
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("Dockspace Demo", &dockspaceOpen, window_flags); // first begin for dockspace demo
            ImGui::PopStyleVar();

            if (opt_fullscreen)
            {
                ImGui::PopStyleVar(2); // Calling it 2 times to reset the opt_fullscreen variables
            }

            // Dockspace
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                // creating a unique Dockspace ID to manage the space
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags); // create the dockspace
            }

            // Menubar and fil->exit function
            if (ImGui::BeginMenuBar())
            {
                if (ImGui::BeginMenu("File"))
                {
                    // Disabling fullscreen would allow the window to be moved to the front of other windows,
                    // which we cannot undo at the moment without finer window depth/z control
                    // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistent);

                    if (ImGui::MenuItem("Exit")) Mashenka::Application::Get().Close();
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

            // The original settings imgui window
            {
                ImGui::Begin("Settings");

                auto stats = Mashenka::Renderer2D::GetStats();
                ImGui::Text("Renderer2D Stats:");
                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quads: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());


                ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

                // getting the RendererID from the framebuffer, and render the texture to the ImGui Window
                uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
                ImGui::Image((void*)textureID, ImVec2{1280, 720});
                ImGui::End();
            }

            // Ending the "Dockspace Demo" ImGui Window
            ImGui::End();
        }
        else // when dockspace is not enabled
        {
            ImGui::Begin("Settings");

            auto stats = Mashenka::Renderer2D::GetStats();
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", stats.DrawCalls);
            ImGui::Text("Quads: %d", stats.QuadCount);
            ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
            ImGui::Text("Indices: %d", stats.GetTotalIndexCount());


            ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

            uint32_t textureID = m_CheckerboardTexture->GetRendererID();
            ImGui::Image((void*)textureID, ImVec2{1280, 720});
            ImGui::End();
        }
    }

    void EditorLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);
        m_CameraController.OnEvent(e); // The camera controller is used to handle events
    }
}
