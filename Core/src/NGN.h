#pragma once

// Main header for editor & runtime apps

#include "Core/Base.h"

#include "Core/Application.h"
#include "Core/Enums.h"
#include "Core/Layer.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Core/Timestep.h"
#include "Core/Types.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
#include "Core/AssetManager.h"

/* Input */
#include "Input/Input.h"

/* ImGui */
#include "ImGui/ImGuiLayer.h"

/* Scene */
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Components.h"
#include "Scene/Systems/PlayerControllerSystem.h"
#include "Scene/SceneCamera.h"

/* Renderer */
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Backend/RenderCommand.h"

#include "Renderer/Resources/Buffer.h"
#include "Renderer/Resources/Shader.h"
#include "Renderer/Resources/Framebuffer.h"
#include "Renderer/Resources/VertexArray.h"
#include "Renderer/Resources/Texture.h"
#include "Renderer/Resources/SubTexture.h"

#include "Renderer/Resources/OrthographicCameraController.h"
#include "Renderer/Resources/PerspCameraController.h"

/* Resources */
#include "ResourceManagement/CPU/Types/Texture.h"