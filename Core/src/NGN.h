#pragma once

// Main header for editor & runtime apps

#include "Core/Base.h"

#include "Core/Application.h"
#include "Core/Layer.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Timestep.h"
#include "Core/Input.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

/* ImGui */
#include "ImGui/ImGuiLayer.h"

/* Scene */
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"

/* Renderer */
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RenderCommand.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"

#include "Renderer/OrthographicCameraController.h"