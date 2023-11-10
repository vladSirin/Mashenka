#pragma once
#include "OrthographicCamera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "glm/gtc/matrix_transform.hpp"
#include "RendererAPI.h"

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

        // Submit the vertex array to the RendererCommand
        // Using shared_ptr to make sure that the object is not deleted when the function is called
        // using shared_ptr reference to make sure that the object is not copied when the function is called, as shaders can be large on data
        // using const reference to make sure that the object is not modified when the function is called, transform matrix is model matrix in rendering
        // Model matrix = Translation * Rotation * Scale
        static void Submit(std::shared_ptr<Shader>& shader, std::shared_ptr<VertexArray> vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

    private:
        // Scene data
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static SceneData* s_SceneData;
    };
};
