#include "EditorLayer.h"

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

        // Create Scene and entity, attach transform and spriteRender Component
        m_ActiveScene = CreateRef<Scene>();

        // Entity Create and Add SpriteRenderComponent
        auto square = m_ActiveScene->CreateEntity("Green Square");
        square.AddComponent<SpriteRenderComponent>(glm::vec4{0.0f, 1.0f, 0.0f, 1.0f});

    	// add red square
    	auto redSquare = m_ActiveScene->CreateEntity("Red Square");
    	redSquare.AddComponent<SpriteRenderComponent>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

        m_SquareEntity = square;

        // Creating main camera
        m_CameraEntity = m_ActiveScene->CreateEntity("Camera A");
        m_CameraEntity.AddComponent<CameraComponent>();
        // Clip Space second camera with normalized projection, use of UI etc.
        m_SecondCamera = m_ActiveScene->CreateEntity("Camera B");
        auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
        cc.Primary = false;


        // Example ScripEntity
        class CameraController : public ScriptEntity
        {
        public:
            virtual void OnCreate() override
            {
            	auto& transform = GetComponent<TransformComponent>().Transform;
            	transform[3][0] = rand() % 10 - 5.0f; // random assign position on x-axis
            }

            virtual void OnDestroy() override
            {
            }

            virtual void OnUpdate(TimeStep ts) override
            {
                auto& transform = GetComponent<TransformComponent>().Transform;
                float speed = 5.0f;

                if (Input::IsKeyPressed(Key::A))
                {
                    transform[3][0] -= speed * ts;
                }
                if (Input::IsKeyPressed(Key::D))
                {
                    transform[3][0] += speed * ts;
                }
                if (Input::IsKeyPressed(Key::W))
                {
                    transform[3][1] += speed * ts;
                }
                if (Input::IsKeyPressed(Key::S))
                {
                    transform[3][1] -= speed * ts;
                }
            }
        };

        // Key Step to add a NativeScriptComponent and bind it with a cameraController Class
        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
    	m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    	m_SceneHierarchyPanel.SetConText(m_ActiveScene);
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

        // Resize
        // Check if the framebuffer size are not matching the viewport size and resize the buffer
        // This is handled in update before rendering to avoid glitch
        if (FramebufferSpecification spec = m_Framebuffer->GetSpecification(); m_ViewportSize.x > 0.0f &&
            m_ViewportSize.y > 0.0f && // zero sized frame buffer is invalid
            (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
        {
            // Resize the framebuffer and camera
            m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        }

        // update camera if focused
        if (m_ViewportFocused)
        {
            m_CameraController.OnUpdate(ts);
        }


        //render
        Renderer2D::ResetStats();
        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        RenderCommand::Clear();

        /*{
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
        }*/


        m_ActiveScene->OnUpdate(ts);
        m_Framebuffer->Unbind();
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

            // Menubar and file->exit function
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
				m_SceneHierarchyPanel.OnImGuiRender();
            	
                ImGui::Begin("Settings");

                auto stats = Mashenka::Renderer2D::GetStats();
                ImGui::Text("Renderer2D Stats:");
                ImGui::Text("Draw Calls: %d", stats.DrawCalls);
                ImGui::Text("Quads: %d", stats.QuadCount);
                ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
                ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

                ImGui::End();

                /* Adapting to the window resize of Imgui viewport.
                 * Maintaining the aspect ratio of the rendered image in an efficient way.
                 * This routine gets the size of the available space of the Imgui viewport and compare it with the
                 * current framebuffer size, if they are not matching, resize the framebuffer to the space
                 * then rendering it without padding
                 */
                {
                    // pushing the style variable to make sure there is no padding for the window
                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
                    ImGui::Begin("Viewport"); // start the viewport

                    // Getting the states for the viewport and BlockEvents if neither state are true
                    m_ViewportFocused = ImGui::IsWindowFocused();
                    m_ViewportHovered = ImGui::IsWindowHovered();
                    Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

                    // retrieve the size of the available content region within the "viewport" window
                    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

                    // Cast viewportPanelSize to glm::vec2* and dereference for comparison
                    m_ViewportSize = {viewportPanelSize.x, viewportPanelSize.y};

                    // Get the texture ID and render it into the viewport
                    uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

                    // Draw the image with the texture from the framebuffer.
                    // converts textureID to void* as this is the format for Imgui to support cross-platform agnostic
                    /* @brief:
                    * static_cast<uintptr_t>(textureID): This converts the uint32_t texture ID to uintptr_t, which is an unsigned integer type capable of holding a pointer.
                    * reinterpret_cast<void*>: This converts the uintptr_t value to a void*, ensuring no loss of information and avoiding the warning.
                    */
                    ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(textureID)),
                                 ImVec2{m_ViewportSize.x, m_ViewportSize.y},
                                 ImVec2{0, 1},
                                 ImVec2{1, 0});
                    ImGui::End();
                    ImGui::PopStyleVar();
                }
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
            ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(textureID)), ImVec2{1280, 720});
            ImGui::End();
        }
    }

    void EditorLayer::OnEvent(Event& e)
    {
        Layer::OnEvent(e);
        m_CameraController.OnEvent(e); // The camera controller is used to handle events
    }
}
