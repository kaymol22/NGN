#include "BackendTestLayer.h"
#include "NGN.h"

BackendTestLayer::BackendTestLayer() 
	: NGN::Layer("TestLayer") 
{
	
}

void BackendTestLayer::OnAttach()
{
	auto scene = NGN::Application::Get().CreateScene("BackendTestScene");
	auto camEntity = scene->CreateEntity("Camera");
	camEntity.AddComponent<NGN::CameraComponent>();
	auto camComponent = camEntity.GetComponent<NGN::CameraComponent>();
	auto camTransform = camEntity.GetComponent<NGN::TransformComponent>();
	camTransform.Translation = { 0.0f, 0.0f, -5.0f };


	auto spriteEntity = scene->CreateEntity("Sprite");
	spriteEntity.AddComponent<NGN::SpriteComponent>("Checkerboard");
	auto spriteTransform = spriteEntity.GetComponent<NGN::TransformComponent>();
	spriteTransform.Translation = { 0.0f, 0.0f, 5.0f };
}

void BackendTestLayer::OnUpdate(NGN::Timestep ts)
{
}

void BackendTestLayer::OnEvent(NGN::Event& e)
{

}