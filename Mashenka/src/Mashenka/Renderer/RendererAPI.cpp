#include "mkpch.h"
#include "RendererAPI.h"

namespace Mashenka
{
    // TODO: setup the renderer API based on the platform
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
}
