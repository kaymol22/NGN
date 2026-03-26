#include "GameLayer.h"

GameLayer::GameLayer() : Layer("GameLayer"), 
m_CameraController(NGN::Application::Get().GetFramebufferSize().x / NGN::Application::Get().GetFramebufferSize().y, true)
{
}

void GameLayer::OnAttach()
{
	NGN_PROFILE_FUNCTION();

	m_CheckerBoardTexture = NGN::Texture2D::Create("assets/Textures/Checkerboard.png");
}

void GameLayer::OnDetach()
{
	NGN_PROFILE_FUNCTION();
}

void GameLayer::OnUpdate(NGN::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	// Render
	NGN::Renderer2D::ResetStats();
	NGN::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	NGN::RenderCommand::Clear();

	static float rotation = 0.0f;
	rotation += ts * 20.0f;

	NGN::Renderer2D::BeginScene(m_CameraController.GetCamera());
	NGN::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerBoardTexture, 10.0f);
	NGN::Renderer2D::DrawQuad({ -5.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, { 1.0f, 0.0f, 0.0f, 1.0f });
	NGN::Renderer2D::DrawRotatedQuad({ 5.0f, 0.0f, -0.1f }, { 2.0f, 2.0f }, rotation, m_CheckerBoardTexture, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	NGN::Renderer2D::DrawRotatedQuad({ 0.0f, 3.0f, -0.1f }, { 3.0f, 3.0f }, { 0.0f, 0.5f, 0.5f, 1.0f }, rotation);
	NGN::Renderer2D::DrawRect({ 0.0f, -3.0f, -0.1f }, { 3.0f, 3.0f }, { 0.5f, 0.5f, 0.0f, 1.0f });
	NGN::Renderer2D::DrawLine({ -5.0f, -5.0f, -0.1f }, { 5.0f, 5.0f, -0.1f }, { 0.0f, 1.0f, 0.0f, 1.0f });
	NGN::Renderer2D::DrawLine({  -5.0f,  5.0f, -0.1f }, { 5.0f, -5.0f, -0.1f }, { 0.0f, 0.0f, 1.0f, 1.0f });
	NGN::Renderer2D::EndScene();
}

void GameLayer::OnEvent(NGN::Event& e)
{
	m_CameraController.OnEvent(e);
}