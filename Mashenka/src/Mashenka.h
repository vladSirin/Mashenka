#pragma once

// For use by Mashenka Application
#include "Mashenka/Core/Application.h"
#include "Mashenka/Core/Layer.h"
#include "Mashenka/Core/Log.h"
#include "Mashenka/Core/Base.h"

// PRODUCT_CORE
#include "Mashenka/Core/Timestep.h"

// INPUT
#include "Mashenka/Core/Input.h"
#include "Mashenka/Core/KeyCodes.h"
#include "Mashenka/Core/MouseCode.h"
#include "Mashenka/Events/KeyEvent.h"
#include "Mashenka/Events/MouseEvent.h"

// GUI
#include "Mashenka/ImGui/ImGuiLayer.h"

// ECS
#include "Mashenka/Scene/Scene.h"
#include "Mashenka/Scene/Component.h"

// --------Renderer-------------------
#include "Mashenka/Renderer/Renderer.h"
#include "Mashenka/Renderer/Renderer2D.h"
#include "Mashenka/Renderer/RenderCommand.h"


#include "Mashenka/Renderer/Buffer.h"
#include "Mashenka/Renderer/Shader.h"
#include "Mashenka/Renderer/Texture.h"
#include "Mashenka/Renderer/VertexArray.h"
#include "Mashenka/Renderer/Framebuffer.h"

// Camera
#include "Mashenka/Renderer/Camera.h"
#include "Mashenka/Renderer/OrthographicCamera.h"
#include "Mashenka/Renderer/OrthographicCameraController.h"

// ---Entry Point---------------------
// #include "Mashenka/Core/EntryPoint.h" //There should only be one entry point