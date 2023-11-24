#pragma once

// For use by Mashenka Application
#include "Mashenka/Application.h"
#include "Mashenka/Layer.h"
#include "Mashenka/Log.h"

// PRODUCT_CORE
#include "Mashenka/Core/Timestep.h"

// INPUT
#include "Mashenka/Input.h"
#include "Mashenka/KeyCodes.h"
#include "Mashenka/MouseButtonCodes.h"
#include "Mashenka/Events/KeyEvent.h"
#include "Mashenka/Events/MouseEvent.h"

// GUI
#include "Mashenka/ImGui/ImGuiLayer.h"

// --------Renderer-------------------
#include "Mashenka/Renderer/Renderer.h"
#include "Mashenka/Renderer/RenderCommand.h"


#include "Mashenka/Renderer/Buffer.h"
#include "Mashenka/Renderer/Shader.h"
#include "Mashenka/Renderer/Texture.h"
#include "Mashenka/Renderer/VertexArray.h"

// Camera
#include "Mashenka/Renderer/Camera.h"
#include "Mashenka/Renderer/OrthographicCamera.h"
#include "Mashenka/OrthographicCameraController.h"

// ---Entry Point---------------------
#include "Mashenka/EntryPoint.h"