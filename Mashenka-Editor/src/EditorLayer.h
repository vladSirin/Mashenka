#pragma once
#include "Mashenka.h"

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

        Ref<Texture2D> m_CheckerboardTexture;
        glm::vec4 m_SquareColor = {0.2 ,0.3, 0.8, 1.0};
        
    };
}
