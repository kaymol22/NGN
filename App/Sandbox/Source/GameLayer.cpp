#include "GameLayer.h"

GameLayer::GameLayer() : Layer("GameLayer"), 
m_Camera()
{
}

void GameLayer::OnAttach()
{
	NGN_PROFILE_FUNCTION();

	m_CheckerBoardTexture = NGN::Texture2D::Create("assets/Textures/Checkerboard.png");
	m_SpriteSheet = NGN::Texture2D::Create("assets/Textures/spritesheet-1.png");

	constexpr glm::vec2 cellSize = { 64.0f, 64.0f };
	m_WaterSprite = NGN::SubTexture2D::CreateFromCoords(m_SpriteSheet,   { 11, 11 }, { 1, 1 }, cellSize);
	m_GrassSprite = NGN::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 1, 1 }, cellSize);
	m_TreeSprite = NGN::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 1, 2 }, cellSize);

	// Look down at textures for now
	m_Camera.SetPosition({ -5.0f, 5.0f, 15.0f });
	m_Camera.SetPitch(15.0f);
	m_Camera.SetYaw(10.0f);
}

void GameLayer::OnDetach()
{
	NGN_PROFILE_FUNCTION();
}

void GameLayer::OnUpdate(NGN::Timestep ts)
{
	/*m_CameraController.OnUpdate(ts);*/

	// Render
	NGN::Renderer2D::ResetStats();
	NGN::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	NGN::RenderCommand::Clear();

	static float rotation = 0.0f;
	rotation += ts * 20.0f;

	NGN::Renderer2D::BeginScene(m_Camera);
	NGN::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerBoardTexture, 10.0f);
	NGN::Renderer2D::DrawQuad({ -3.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_GrassSprite);
	NGN::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, { 1.0f, 2.0f }, m_TreeSprite);
	NGN::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, m_WaterSprite);
	NGN::Renderer2D::DrawRotatedQuad({ 5.0f, 0.0f, -0.1f }, { 1.0f, 1.0f }, rotation, m_WaterSprite, 1.0f, { 1.0f, 1.0f, 1.0f, 1.0f });
	NGN::Renderer2D::DrawRotatedQuad({ 0.0f, 3.0f, -0.1f }, { 3.0f, 3.0f }, { 0.0f, 0.5f, 0.5f, 1.0f }, rotation);
	NGN::Renderer2D::EndScene();
}

void GameLayer::OnEvent(NGN::Event& e)
{
	/*m_CameraController.OnEvent(e);*/

	if (e.GetEventType() == NGN::EventType::KeyPressed)
	{
		NGN::KeyPressedEvent& keyEvent = static_cast<NGN::KeyPressedEvent&>(e);
		if (keyEvent.GetKeyCode() == NGN::Key::T)
			NGN_INFO("T Key pressed");
	}
}