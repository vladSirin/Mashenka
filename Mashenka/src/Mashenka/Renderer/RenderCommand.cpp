#include "mkpch.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Mashenka
{
    // Initialize the static member variable
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
