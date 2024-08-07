﻿#pragma once
#include "Mashenka.h"
#include "Panels/SceneHierarchyPanel.h"

namespace Mashenka
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void OnUpdate(TimeStep ts) override;
        virtual void OnImGuiRender() override;
        void OnEvent(Event& e) override;

    private:
        Mashenka::OrthographicCameraController m_CameraController;

        // TEMP
        Ref<VertexArray> m_SquareVA;
        Ref<Shader> m_FlatColorShader;
        Ref<Framebuffer> m_Framebuffer;

        Ref<Scene> m_ActiveScene;
        Entity m_SquareEntity;
        Entity m_CameraEntity;
        Entity m_SecondCamera;

        bool m_PrimaryCamera = true;

        Ref<Texture2D> m_CheckerboardTexture;
        bool m_ViewportFocused = false, m_ViewportHovered = false;
        glm::vec2 m_ViewportSize = {0.0f, 0.0f}; // used for sizing to match the viewport of imgui
        glm::vec4 m_SquareColor = {0.2 ,0.3, 0.8, 1.0};

    	// panels
    	SceneHierarchyPanel m_SceneHierarchyPanel;
        
    };
}
