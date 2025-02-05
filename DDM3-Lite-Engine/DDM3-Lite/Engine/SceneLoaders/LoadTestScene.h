#include "../../Vulkan/VulkanRenderer.h"
#include "../../Managers/SceneManager.h"

#include "../../DataTypes/Materials/Material.h"
#include "../../DataTypes/Materials/TexturedMaterial.h"
#include "../../DataTypes/Materials/MultiMaterial.h"

#include "../../Components/MeshRenderComponent.h"
#include "../../DataTypes/Mesh.h"
#include "../../Components/TransformComponent.h"
#include "../Scene.h"
#include "../../Components/SkyBoxComponent.h"

#include "../../Components/CameraComponent.h"
#include "../../Components/DirectionalLightComponent.h"

#include "../../Components/RotatorComponent.h"
#include "../../Components/SpectatorMovementComponent.h"
#include "../../Components/AnimatorComponent.h"

#include "../../Vulkan/VulkanRenderer.h"
#include "../../Managers/ResourceManager.h"
#include "../../Managers/ConfigManager.h"

namespace LoadTestScene
{
	void SetupPipelines();

	void SetupVehicle(DDM3::Scene* scene);

	void SetupVikingRoom(DDM3::Scene* scene);

	void SetupGun(DDM3::Scene* scene);

	void SetupMario(DDM3::Scene* scene);

	void SetupCamera(DDM3::Scene* scen);

	void SetupLight(DDM3::Scene* scene);

	void SetupGroundPlane(DDM3::Scene* scene);

	void loadTestScene()
	{
		auto scene = DDM3::SceneManager::GetInstance().CreateScene("Test");
		DDM3::SceneManager::GetInstance().SetActiveScene(scene);

		SetupPipelines();

		SetupGroundPlane(scene.get());

		SetupVehicle(scene.get());

		//SetupVikingRoom(scene.get());

		//SetupGun(scene.get());

		SetupMario(scene.get());

		SetupCamera(scene.get());

		SetupLight(scene.get());
	}

	void SetupPipelines()
	{
		auto& renderer{ DDM3::VulkanRenderer::GetInstance() };

		renderer.AddGraphicsPipeline("Diffuse", { "Resources/Shaders/Diffuse.Vert.spv", "Resources/Shaders/Diffuse.Frag.spv" });
		renderer.AddGraphicsPipeline("NormalMap", { "Resources/Shaders/NormalMap.Vert.spv", "Resources/Shaders/NormalMap.Frag.spv" });
		renderer.AddGraphicsPipeline("DiffNorm", { "Resources/Shaders/DiffNorm.Vert.spv", "Resources/Shaders/DiffNorm.Frag.spv" });
		renderer.AddGraphicsPipeline("Test", { "Resources/Shaders/Test.Vert.spv", "Resources/Shaders/Test.Frag.spv" });
		renderer.AddGraphicsPipeline("DiffuseUnshaded", { "Resources/Shaders/DiffuseUnshaded.Vert.spv", "Resources/Shaders/DiffuseUnshaded.Frag.spv" });
		renderer.AddGraphicsPipeline("Specular", { "Resources/Shaders/Specular.Vert.spv", "Resources/Shaders/Specular.Frag.spv" });
		renderer.AddGraphicsPipeline("DiffNormSpec", { "Resources/Shaders/DiffNormSpec.Vert.spv", "Resources/Shaders/DiffNormSpec.Frag.spv" });
		renderer.AddGraphicsPipeline("MultiShader", { "Resources/Shaders/MultiShader.Vert.spv", "Resources/Shaders/MultiShader.Frag.spv" });
		renderer.AddGraphicsPipeline("Skinned", { "Resources/Shaders/Skinned.Vert.spv", "Resources/Shaders/MultiShader.Frag.spv" });
	}

	void SetupVehicle(DDM3::Scene* scene)
	{
		std::shared_ptr<DDM3::MultiMaterial> pVehicleMaterial{ std::make_shared<DDM3::MultiMaterial>("MultiShader") };

		pVehicleMaterial->AddDiffuseTextures(std::initializer_list<const std::string>{"resources/images/vehicle_diffuse.png"});

		pVehicleMaterial->AddNormalMap(std::initializer_list<const std::string>{"resources/images/vehicle_normal.png"});

		pVehicleMaterial->AddGlossTextures(std::initializer_list<const std::string>{"resources/images/vehicle_gloss.png"});

		pVehicleMaterial->AddSpecularTextures(std::initializer_list<const std::string>{"resources/images/vehicle_specular.png"});

		auto pVehicle{ scene->CreateGameObject("Vehicle") };
		pVehicle->SetShowImGui(true);
		//pVehicle->AddComponent<D3D::RotatorComponent>();

		auto pVehicleMesh{ DDM3::ResourceManager::GetInstance().LoadMesh("Resources/Models/vehicle.obj") };

		auto pVehicleModel{ pVehicle->AddComponent<DDM3::MeshRenderComponent>() };
		pVehicleModel->SetMesh(pVehicleMesh);
		pVehicleModel->SetMaterial(pVehicleMaterial);


		auto pVehicleTransform{ pVehicle->GetTransform() };
		pVehicleTransform->SetShowImGui(true);
		pVehicleTransform->SetLocalPosition(0, 3, 0);
		pVehicleTransform->SetLocalRotation(0.f, glm::radians(75.0f), 0.f);
		pVehicleTransform->SetLocalScale(0.05f, 0.05f, 0.05f);
	}

	void SetupVikingRoom(DDM3::Scene* scene)
	{
		std::shared_ptr<DDM3::TexturedMaterial> pVikingMaterial{ std::make_shared<DDM3::TexturedMaterial>(std::initializer_list<const std::string>{"resources/images/viking_room.png"}, "Diffuse") };

		auto pvikingRoom{ scene->CreateGameObject("Viking Room") };

		auto pVikingRoomMesh{ DDM3::ResourceManager::GetInstance().LoadMesh("Resources/Models/viking_room.obj") };

		auto pVikingRoomModel{ pvikingRoom->AddComponent<DDM3::MeshRenderComponent>() };
		pVikingRoomModel->SetMesh(pVikingRoomMesh);
		pVikingRoomModel->SetMaterial(pVikingMaterial);

		auto pVikingTransform{ pvikingRoom->GetTransform() };
		pVikingTransform->SetLocalPosition(1.f, -0.2f, 3.f);
		pVikingTransform->SetLocalRotation(glm::radians(-90.0f), glm::radians(45.0f), 0.f);
		pVikingTransform->SetLocalScale(0.75f, 0.75f, 0.75f);
	}

	void SetupGun(DDM3::Scene* scene)
	{
		std::shared_ptr<DDM3::MultiMaterial> pGunMaterial{ std::make_shared<DDM3::MultiMaterial>("MultiShader") };

		pGunMaterial->AddDiffuseTextures(std::initializer_list<const std::string>{"resources/images/gun_BaseColor.png"});

		pGunMaterial->AddNormalMap(std::initializer_list<const std::string>{"resources/images/gun_Normal.png"});

		auto pGun{ scene->CreateGameObject("Gun") };

		auto pGunMesh{ DDM3::ResourceManager::GetInstance().LoadMesh("Resources/Models/gun.fbx") };

		auto pGunModel{ pGun->AddComponent<DDM3::MeshRenderComponent>() };
		pGunModel->SetMesh(pGunMesh);
		pGunModel->SetMaterial(pGunMaterial);

		auto pGunTransform{ pGun->GetTransform() };
		pGunTransform->SetLocalPosition(0.f, -.5f, 6.f);
		pGunTransform->SetLocalRotation(0.f, glm::radians(-90.f), 0.f);
		pGunTransform->SetLocalScale(0.5f, 0.5f, 0.5f);
	}

	void SetupMario(DDM3::Scene* scene)
	{
		auto pMario{ scene->CreateGameObject("Mario") };

		auto pMarioMesh{ DDM3::ResourceManager::GetInstance().LoadMesh("Resources/Models/MarioDancing.fbx") };

		auto pMarioModel{ pMario->AddComponent<DDM3::MeshRenderComponent>() };
		pMarioModel->SetMesh(pMarioMesh);

		auto pMarioAnimationComponent{ pMario->AddComponent<DDM3::AnimatorComponent>() };
		pMarioAnimationComponent->AddAnimations("Resources/Models/MarioDancing.fbx");

		auto pMarioTransform{ pMario->GetTransform() };
		pMarioTransform->SetLocalPosition(0.f, 0.f, 2);
		pMarioTransform->SetLocalRotation(0.f, glm::radians(180.f), 0.f);
		//pMarioTransform->SetLocalScale(0.5f, 0.5f, 0.5f);
	}

	void SetupCamera(DDM3::Scene* scene)
	{
		auto pCamera{ scene->CreateGameObject("Camera") };

		pCamera->AddComponent<DDM3::SpectatorMovementComponent>();

		auto pCameraComponent{ pCamera->AddComponent<DDM3::CameraComponent>() };

		auto pCameraTransform{ pCamera->GetTransform() };
		pCameraTransform->SetLocalPosition(0, 5, -5);
		//pCameraTransform->SetLocalRotation(glm::vec3(0.0f, glm::radians(180.f), 0.0f));

		//pCamera->AddComponent<D3D::RotatorComponent>();

		scene->SetCamera(pCameraComponent);

		auto& configManager{ DDM3::ConfigManager::GetInstance() };

		// Set the vertex shader name
		const std::string vertShaderName{configManager.GetString("SkyboxVertName")};
		// Set the fragment shader name
		const std::string fragShaderName{ configManager.GetString("SkyboxFragName") };

		// Create the graphics pipeline for the skybox
		DDM3::VulkanRenderer::GetInstance().AddGraphicsPipeline(configManager.GetString("SkyboxPipelineName"), {vertShaderName, fragShaderName}, false);

		auto pSkyBox{ pCamera->AddComponent<DDM3::SkyBoxComponent>() };

		pSkyBox->LoadSkybox(std::initializer_list<const std::string>{"resources/images/CubeMap/Sky_Right.png",
			"resources/images/CubeMap/Sky_Left.png",
			"resources/images/CubeMap/Sky_Up.png",
			"resources/images/CubeMap/Sky_Down.png",
			"resources/images/CubeMap/Sky_Front.png",
			"resources/images/CubeMap/Sky_Back.png"});
	}
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
	void SetupLight(DDM3::Scene* scene)
	{
		auto pLight{ scene->CreateGameObject("Light") };
		pLight->SetShowImGui(true);

		auto pLightComponent{ pLight->AddComponent<DDM3::DirectionalLightComponent>() };
		pLightComponent->SetShowImGui(true);

		//pLightComponent->SetColor(glm::vec3{ 0, 0, 0 });

		auto pLightTransform{ pLight->GetTransform() };
		pLightTransform->SetShowImGui(true);
		pLightTransform->SetLocalRotation(glm::vec3(glm::radians(45.f), glm::radians(45.f), 0.0f));


		//pLight->AddComponent<D3D::RotatorComponent>();

		scene->SetLight(pLightComponent);
	}

	void SetupGroundPlane(DDM3::Scene* scene)
	{
		std::shared_ptr<DDM3::MultiMaterial> pFloorMaterial{ std::make_shared<DDM3::MultiMaterial>("MultiShader") };


		//std::shared_ptr<D3D::TexturedMaterial> pFloorMaterial{
		//	std::make_shared<D3D::TexturedMaterial>(std::initializer_list<const std::string>{"resources/images/GroundPlane.png"}, "DiffuseUnshaded") };

		pFloorMaterial->AddDiffuseTextures(std::initializer_list<const std::string>{"resources/images/GroundPlane.png"});

		auto pGroundPlane{ scene->CreateGameObject("Ground Plane") };
		pGroundPlane->SetShowImGui(true);

		auto pGroundplaneMesh{ DDM3::ResourceManager::GetInstance().LoadMesh("Resources/Models/Plane.obj") };

		auto pGroundPlaneModel{ pGroundPlane->AddComponent<DDM3::MeshRenderComponent>() };
		pGroundPlaneModel->SetShowImGui(true);
		pGroundPlaneModel->SetMesh(pGroundplaneMesh);
		pGroundPlaneModel->SetMaterial(pFloorMaterial);
	}
}