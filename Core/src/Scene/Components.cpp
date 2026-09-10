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
		Texture = NGN::Application::Get().GetResourceManager().Load<RS::Texture>(id);
	}

	MeshComponent::MeshComponent(const std::string& id, const glm::vec4& color)
	{
		GenericMesh = NGN::Application::Get().GetResourceManager().Load<RS::GenericMesh>(id);
		Color = color;

		boundsMin = GenericMesh.Get()->GetBoundsMin();
		boundsMax = GenericMesh.Get()->GetBoundsMax();
	}
}