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
		Mesh = NGN::Application::Get().GetResourceManager().Load<RS::MeshAsset>(id);
		Color = color;
		SubMeshes = Mesh.Get()->GetSubMeshes();
		boundsMin = Mesh.Get()->GetBoundsMin();
		boundsMax = Mesh.Get()->GetBoundsMax();
	}

	CameraComponent::CameraComponent() : Camera() {
		Camera.SetViewportSize(NGN::Application::Get().GetWindow().GetWidth(), NGN::Application::Get().GetWindow().GetHeight());
	}
}