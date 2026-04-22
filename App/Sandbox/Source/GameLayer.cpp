#include "GameLayer.h"

GameLayer::GameLayer() : Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
	// Scene Loading + setup 
	NGN_PROFILE_FUNCTION();

	auto scene = NGN::CreateRef<NGN::Scene>();
	NGN::Application::Get().GetSceneManager().RegisterScene("GameScene", scene);
	NGN::Application::Get().GetSceneManager().SetActiveScene("GameScene");

	// Texture + sprite setup

	m_CheckerBoardTexture = NGN::Texture2D::Create("assets/Textures/Checkerboard.png");
	m_SpriteSheet = NGN::Texture2D::Create("assets/Textures/spritesheet-1.png");
	

	constexpr glm::vec2 cellSize = { 64.0f, 64.0f };
	m_WaterSprite = NGN::SubTexture2D::CreateFromCoords(m_SpriteSheet,   { 11, 11 }, { 1, 1 }, cellSize);
	m_GrassSprite = NGN::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1, 11 }, { 1, 1 }, cellSize);
	m_TreeSprite = NGN::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 0, 1 }, { 1, 2 }, cellSize);

	// Entity Creation

	auto cameraEntity = scene->CreateEntity("Player");
	auto& cameraTrans = cameraEntity.GetComponent<NGN::TransformComponent>();
	cameraTrans.Translation = { 0.0f, 0.0f, 10.0f };

	cameraEntity.AddComponent<NGN::CameraComponent>();
	auto& cameraComp = cameraEntity.GetComponent<NGN::CameraComponent>();
	cameraComp.Camera.SetViewportSize(
		NGN::Application::Get().GetWindow().GetWidth(),
		NGN::Application::Get().GetWindow().GetHeight()
	);
	cameraComp.Camera.SetProjectionType(NGN::ProjectionType::Orthographic);
	/*cameraEntity.AddComponent<NGN::PlayerControllerComponent>();*/

	auto bgEntity = scene->CreateEntity("Background");
	bgEntity.AddComponent<NGN::SpriteRendererComponent>(m_CheckerBoardTexture);
	bgEntity.GetComponent<NGN::TransformComponent>().Translation = { 0.0f, 0.0f, -2.0f };
	bgEntity.GetComponent<NGN::TransformComponent>().Scale = { 10.0f, 10.0f, 1.0f };
	bgEntity.GetComponent<NGN::SpriteRendererComponent>().TilingFactor = 5.0f;

	auto grassEntity = scene->CreateEntity("Grass");
	grassEntity.AddComponent<NGN::SpriteRendererComponent>(m_GrassSprite);
	grassEntity.GetComponent<NGN::TransformComponent>().Translation = { 0.0f, 0.0f, -0.1f };
	grassEntity.GetComponent<NGN::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };

	auto treeEntity = scene->CreateEntity("Tree");
	treeEntity.AddComponent<NGN::SpriteRendererComponent>(m_TreeSprite);
	treeEntity.GetComponent<NGN::TransformComponent>().Translation = { -3.0f, 0.0f, -0.1f };
	treeEntity.GetComponent<NGN::TransformComponent>().Scale = { 1.0f, 2.0f, 1.0f };

	auto waterEntity = scene->CreateEntity("Water");
	waterEntity.AddComponent<NGN::SpriteRendererComponent>(m_WaterSprite);
	waterEntity.GetComponent<NGN::TransformComponent>().Translation = { 1.0f, 0.0f, -0.1f };
	waterEntity.GetComponent<NGN::TransformComponent>().Scale = { 1.0f, 1.0f, 1.0f };
}

void GameLayer::OnDetach()
{
	NGN_PROFILE_FUNCTION();
}

void GameLayer::OnUpdate(NGN::Timestep ts)
{
	/*m_CameraController.OnUpdate(ts);*/
	auto scene = NGN::Application::Get().GetSceneManager().GetActiveScene();
	if (!scene)
		return;

	scene->OnUpdate(ts);

	// Render - temp until renderer integrated into scene + render system
	NGN::Renderer2D::ResetStats();
	NGN::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	NGN::RenderCommand::Clear();

	scene->RenderScene();
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