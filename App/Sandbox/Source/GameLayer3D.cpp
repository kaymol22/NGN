#include "NGN.h"
#include "GameLayer3D.h"

GameLayer3D::GameLayer3D()
	: NGN::Layer("GameLayer3D")
{
}

void GameLayer3D::OnAttach()
{
	NGN_PROFILE_FUNCTION();

	auto scene = NGN::Application::Get().CreateScene("GameScene3D");

	// Create 3D shader
	auto meshShader = NGN::Shader::Create("assets/Shaders/Mesh3D.glsl");
	NGN_INFO("GameLayer3D: Shader loaded");

	// Player Entity
	auto cameraEntity = scene->CreateEntity("Player");
	auto& cameraTrans = cameraEntity.GetComponent<NGN::TransformComponent>();
	cameraTrans.Translation = { 0.0f, 0.0f, 10.0f };

	cameraEntity.AddComponent<NGN::CameraComponent>();
	auto& cameraComp = cameraEntity.GetComponent<NGN::CameraComponent>();
	cameraComp.Camera.SetViewportSize(
		NGN::Application::Get().GetWindow().GetWidth(),
		NGN::Application::Get().GetWindow().GetHeight()
	);
	cameraComp.Camera.SetPerspective(45.0f, 16.0f / 9.0f, 0.1f, 100.0f);
	NGN_INFO("GameLayer3D: Camera created at (0, 0, 10)");

	// Mesh Entity - Cube
	auto cubeMesh = NGN::Mesh::CreateCube(2.0f);
	NGN_INFO("GameLayer3D: Cube mesh created");

	auto meshEntity = scene->CreateEntity("Cube");
	meshEntity.AddComponent<NGN::MeshComponent>(cubeMesh, meshShader, glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
	auto& meshTransform = meshEntity.GetComponent<NGN::TransformComponent>();
	meshTransform.Translation = { -3.0f, -2.0f, 0.0f };
	meshTransform.Rotation = glm::quat(glm::vec3(0.0f, glm::radians(30.0f), 0.0f));
	NGN_INFO("GameLayer3D: Mesh entity created with component");
}

void GameLayer3D::OnDetach()
{

}

void GameLayer3D::OnEvent(NGN::Event& e)
{
	auto scene = NGN::Application::Get().GetSceneManager().GetActiveScene();
	auto cameraEntity = scene->GetPrimaryCamera();
	auto& camComp = cameraEntity.GetComponent<NGN::CameraComponent>();

	if (e.GetEventType() == NGN::EventType::KeyPressed)
	{
		NGN::KeyPressedEvent& keyEvent = static_cast<NGN::KeyPressedEvent&>(e);
		if (keyEvent.GetKeyCode() == NGN::Key::O)
		{
			NGN_INFO("Switching to Orthographic");
			camComp.Camera.SetOrthographic(
				camComp.Camera.GetOrthographicSize(),
				camComp.Camera.GetOrthographicNear(),
				camComp.Camera.GetOrthographicFar());
		}

		if (keyEvent.GetKeyCode() == NGN::Key::P)
		{
			NGN_INFO("Switching to Perspective");
			camComp.Camera.SetPerspective(
				camComp.Camera.GetPerspectiveFOV(),
				16.0f / 9.0f,
				camComp.Camera.GetPerspectiveNear(),
				camComp.Camera.GetPerspectiveFar());
		}
	}
}

void GameLayer3D::OnUpdate(NGN::Timestep ts)
{
	auto scene = NGN::Application::Get().GetSceneManager().GetActiveScene();
	if (!scene)
		return;

	scene->OnUpdate(ts);

	// Clear and render scene
	NGN::Renderer2D::ResetStats();
	NGN::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	NGN::RenderCommand::Clear();

	scene->RenderScene();
}
