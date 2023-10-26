#pragma once
#include "RendererAPI.h"
#include "VertexArray.h"

namespace Mashenka
{
    // high-level rendering functions used by application
    // This class  will stay high level and will not be platform-specific
    // This mostly will be used to handle shader and scene management for application
    class Renderer
    {
    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<VertexArray> vertexArray);
        
    };
};
