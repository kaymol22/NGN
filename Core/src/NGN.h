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

/* Resources */
#include "ResourceManagement/CPU/Types/Texture.h"