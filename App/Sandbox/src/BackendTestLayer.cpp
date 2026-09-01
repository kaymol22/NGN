#include "BackendTestLayer.h"

BackendTestLayer::BackendTestLayer() 
	: NGN::Layer("TestLayer") 
{
	
}

void BackendTestLayer::OnAttach()
{
	auto scene = NGN::Application::Get().CreateScene("BackendTestScene");
	scene->CreateEntity("Test");
	auto texHandle = NGN::Application::Get().GetResourceManager().Load<RS::Texture>("Checkerboard");
}

void BackendTestLayer::OnUpdate(NGN::Timestep ts)
{
	
}

void BackendTestLayer::OnEvent(NGN::Event& e)
{

}