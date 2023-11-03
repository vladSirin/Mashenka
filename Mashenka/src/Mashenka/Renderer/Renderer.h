#pragma once
#include "RendererAPI.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Mashenka/OrthographicCamera.h"

namespace Mashenka
{
    // high-level rendering functions used by application
    // This class  will stay high level and will not be platform-specific
    // This mostly will be used to handle shader and scene management for application
    class MASHENKA_API Renderer
    {
    public:
        static void BeginScene(OrthographicCamera& camera); //Prepare the scene 
        static void EndScene();

        static void Submit(std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray> vertexArray);

    private:
        // Scene data
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
};
