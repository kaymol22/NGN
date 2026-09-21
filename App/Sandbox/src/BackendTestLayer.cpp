#include "BackendTestLayer.h"
#include "NGN.h"

BackendTestLayer::BackendTestLayer() 
	: NGN::Layer("TestLayer") 
{

}

void BackendTestLayer::OnAttach()
{
	NGN::AssetManifest manifest = NGN::AssetManifest();
	/*manifest.Add(NGN::AssetType::Texture, "Button");

	auto& resourceManager = NGN::Application::Get().GetResourceManager();
	auto& assetLoader = NGN::Application::Get().GetAssetLoader();
	assetLoader.LoadAll(resourceManager, manifest);*/

	const auto scene = NGN::Application::Get().CreateScene("BackendTestScene");

	auto camEntity = scene->CreateEntity("Camera");
	auto& camComponent = camEntity.AddComponent<NGN::CameraComponent>();

	auto cube_r = scene->CreateEntity("RedCubeEntity");
	auto& cube_r_Transform = cube_r.GetComponent<NGN::TransformComponent>();
	auto& cube_r_Mesh = cube_r.AddComponent<NGN::MeshComponent>("DefaultCube");
	cube_r_Transform.SetTranslation({ 0.0f, 0.0f, -10.0f });
	cube_r_Mesh.Color = { 1.0f, 0.0f, 0.0f, 1.0f };

	auto cube_b = scene->CreateEntity("BlueCubeEntity");
	auto& cube_b_Transform = cube_b.GetComponent<NGN::TransformComponent>();
	auto& cube_b_Mesh = cube_b.AddComponent<NGN::MeshComponent>("DefaultCube");
	cube_b_Transform.SetTranslation({ 0.0f, 0.0f, 7.0f });
	cube_b_Transform.SetScale({ 0.5f, 0.5f, 0.5f });
	cube_b_Mesh.Color = { 0.0f, 0.0f, 1.0f, 1.0f };
}

void BackendTestLayer::OnUpdate(NGN::Timestep ts)
{
	const auto& scene = NGN::Application::Get().GetSceneManager().GetActiveScene();

	auto camEnt = scene->GetPrimaryCamera();
	auto& camTransform = camEnt.GetComponent<NGN::TransformComponent>();

	const float yaw = glm::radians(30.0f) * ts;

	camTransform.RotateEuler({ 0.0f, yaw, 0.0f });

	scene->OnUpdate(ts);	
}

void BackendTestLayer::OnEvent(NGN::Event& e)
{
	
}