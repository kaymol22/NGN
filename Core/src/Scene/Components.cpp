#include "ngnpch.h"
#include "Components.h"
#include "Core/Application.h"

namespace NGN
{
	SpriteComponent::SpriteComponent(const glm::vec4& color) : Color(color)
	{
		Texture = NGN::Application::Get().GetResourceManager().GetDefaultTexture();
	}

	SpriteComponent::SpriteComponent(const std::string& id)
	{
		// TODO load texture from ID - using default for now
		Texture = NGN::Application::Get().GetResourceManager().GetDefaultTexture();
	}
}