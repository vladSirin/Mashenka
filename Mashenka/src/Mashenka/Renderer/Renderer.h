#pragma once
#include "Mashenka/Renderer/OrthographicCamera.h"
#include "Mashenka/Renderer/Shader.h"
#include "Mashenka/Renderer/RenderCommand.h"

namespace Mashenka
{
    // high-level rendering functions used by application
    // This class  will stay high level and will not be platform-specific
    // This mostly will be used to handle shader and scene management for application
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown(); // clean up the renderer
        // on window resize
        static void OnWindowResize(uint32_t width, uint32_t height);
        static void BeginScene(OrthographicCamera& camera); //Prepare the scene 
        static void EndScene();

        // Submit the vertex array to the RendererCommand
        // Using shared_ptr to make sure that the object is not deleted when the function is called
        // using shared_ptr reference to make sure that the object is not copied when the function is called, as shaders can be large on data
        // using const reference to make sure that the object is not modified when the function is called, transform matrix is model matrix in rendering
        // Model matrix = Translation * Rotation * Scale
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

        // Get API
        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
        
    private:
        // Scene data
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> s_SceneData;
    };
};
