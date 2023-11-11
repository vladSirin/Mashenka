#pragma once
#include "VertexArray.h"
#include "glm/vec4.hpp"

namespace Mashenka
{
    // abstract class to be implemented in the platform-specific code
    // this class will be used by the Renderer class, which is used by the application
    class RendererAPI
    {
    public:
        virtual ~RendererAPI() = default;

        // enum class to specify the API
        enum class API
        {
            None = 0, OpenGL = 1
        };

    public:
        // init
        virtual void Init() = 0;
        
        // virtual functions to be implemented in the platform-specific code
        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) = 0;

        inline static API GetAPI() { return s_API; }

    private:
        static API s_API;
        
    };
}

