#include "mkpch.h"
#include "Mashenka/Renderer/RenderCommand.h"

namespace Mashenka
{
    // Initialize the static member variable
    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
