#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <Debugging/DebugDrawComponents.hpp>
#include <Debugging/DebugDrawSystem.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Input/InputWorldComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/SkyboxWorldComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Resources/ResourceManager.hpp>
#include <Time/TimeWorldComponent.hpp>
#include <UI/ScreenSpaceTextComponent.hpp>

using namespace Poly;

void GameManagerSystem::Init(Scene* scene)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();	

	srand(42);
	
	CreateCamera(scene);

	// gameMgrCmp->GameEntities.PushBack(Ground = CreateModel(scene, "Models/Ground/Ground.fbx"));

	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(scene, "HDR/HDR.hdr", eResourceSource::GAME);
	
	// gameMgrCmp->Model = CreateModel(scene, "Models/leather_shoes/Leather_Shoes.obj");
	// gameMgrCmp->Model->GetTransform().SetGlobalTranslation(Vector(500.0f, 0.0f, 0.0f));

	// gameMgrCmp->Model = CreateModel(scene, "Models/kv-2-heavy-tank-1940/model.obj");
	// gameMgrCmp->Model->GetTransform().SetGlobalScale(Vector(5.0f, 5.0f, 5.0f));
	// gameMgrCmp->Model->GetTransform().SetGlobalTranslation(Vector(0.0f, 10.0f, 0.0f));

	// gameMgrCmp->Model = CreateModel(scene, "Models/1972-datsun-240k-gt/model.obj");
	// gameMgrCmp->Model->GetTransform().SetGlobalScale(Vector::ONE * 20.0f);

	CreatePBRShpereGrid(scene, Vector(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f, 1.0f));
	CreatePBRShpereGrid(scene, Vector(-300.0f, 0.0f, 0.0f), Color(0.5f, 0.5f, 0.5f, 1.0f));
	CreatePBRShpereGrid(scene, Vector(-600.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));

	// CreateSponza(scene);

	CreateTextUI(scene);

	// CreateTranslucent(scene);

	CreatePointLights(scene, 128);
}

void GameManagerSystem::CreateTextUI(Scene* scene)
{
	Entity* text = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<ScreenSpaceTextComponent>(
		scene,
		text,
		Vector2i(50, 50),
		"Fonts/Raleway/Raleway-Regular.ttf",
		eResourceSource::ENGINE,
		40.0f,
		"powered by PolyEngine"
	);
}

Entity* GameManagerSystem::CreateModel(Scene* scene, String path)
{
	Entity* model = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(scene, model, eDebugDrawPreset::STATIC);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, model, path, eResourceSource::GAME);
	return model;
}

void GameManagerSystem::CreateCamera(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 35_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);
	gameMgrCmp->PostCmp->Exposure = 1.0f;
	gameMgrCmp->PostCmp->DOFSize = 1.0f;
	gameMgrCmp->PostCmp->DOFPoint = 300.0f;
	gameMgrCmp->PostCmp->DOFRange = 200.0f;
	// gameMgrCmp->PostCmp->DOFShow = 1.0f;
	gameMgrCmp->PostCmp->BloomScale = 1.0f;

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-550.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));
	gameMgrCmp->Camera = camera;

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, keyDirLight, Color(1.0f, 1.0f, 1.0f), 5.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);
}

void GameManagerSystem::CreatePBRShpereGrid(Scene* scene, Vector pos, Color albedo)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	// Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 50_deg, 1.0f, 5000.f);
	// DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 10.0f, 0.003f);
	// DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);
	// EntityTransform& cameraTrans = camera->GetTransform();
	// cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	// cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	// scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, keyDirLight, Color(1.0f, 1.0f, 1.0f), 1.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);
	// Entity* fillDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, fillDirLight, Color(0.75f, 0.95f, 1.0f), 0.1f);
	// fillDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg + 180_deg) * Quaternion(Vector::UNIT_X, 65_deg + 180_deg));
	// gameMgrCmp->GameEntities.PushBack(fillDirLight);

	// Entity* ground = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// EntityTransform& groundTrans = ground->GetTransform();
	// groundTrans.SetGlobalTranslation(Vector(0.0f, 0.0f, 0.0f));	
	// MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, ground, "Models/Ground/Ground.fbx", eResourceSource::GAME);
	// meshCmp->SetShadingModel(eShadingModel::PBR);
	// int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
	// for (int i = 0; i < materialsNum; ++i)
	// {
	// 	meshCmp->SetPBRMaterial(i, material);
	// }
	// gameMgrCmp->GameEntities.PushBack(ground);

	for (int z = 0; z < 5; ++z)
	{
		for (int y = 0; y < 5; ++y)
		{
			Entity* sphere = DeferredTaskSystem::SpawnEntityImmediate(scene);
			EntityTransform& sphereTrans = sphere->GetTransform();
			sphereTrans.SetGlobalTranslation(pos + Vector(50.0f * y, 0.0f, 50.0f * z) - (Vector(50.0f * 5.0f, -100.0f, 50.0f * 5.0f) * 0.5f));
			sphereTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f) * 20.0f);
			sphereTrans.SetLocalRotation(Quaternion(Vector::UNIT_Z, 90.0_deg));
			MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sphere, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
			// MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sphere, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
			size_t materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
			for (size_t i = 0; i < materialsNum; ++i)
			{
				float Roughness	= ((0.01f + z) / 5.0f);
				float Metallic	= ((0.01f + y) / 5.0f);
				meshCmp->SetMaterial(i, Material(
					Color(0.0f, 0.0f, 0.0f, 1.0f),
					albedo,
					Roughness,
					Metallic,
					0.5f
				));
				meshCmp->SetBlendingMode(eBlendingMode::OPAUQE);
			}

			gameMgrCmp->GameEntities.PushBack(sphere);
		}
	}
}

void GameManagerSystem::CreateTranslucent(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Material material(
		Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f, 0.5f),
		0.1f,
		1.0f,
		0.5f
	);

	for (int i = 0; i < 8; ++i)
	{
		Entity* translucent = DeferredTaskSystem::SpawnEntityImmediate(scene);
		EntityTransform& translucentTrans = translucent->GetTransform();
		Vector randomOffset = RandomVectorRange(-1.0f, 1.0f) * 200.0f;
		translucentTrans.SetGlobalTranslation(Vector(0.0f, 200.0f, 0.0f) + randomOffset);
		translucentTrans.SetLocalScale(Vector(100.0f, 100.0f, 100.0f));
		MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, translucent, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
		size_t materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
		for (size_t i = 0; i < materialsNum; ++i)
		{
			meshCmp->SetMaterial(i, material);
			meshCmp->SetBlendingMode(eBlendingMode::TRANSLUCENT);
		}

		gameMgrCmp->GameEntities.PushBack(translucent);
	}
}

void GameManagerSystem::CreateSponza(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	// Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// CameraComponent* cameraCmp = DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 50_deg, 1.0f, 5000.f);
	// cameraCmp->SetRenderingMode(eRenderingModeType::IMMEDIATE_DEBUG);
	// DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 10.0f, 0.003f);
	// EntityTransform& cameraTrans = camera->GetTransform();
	// cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	// cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	// scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, keyDirLight, Color(0.1f, 0.095f, 0.075f), 1.0f);
	// EntityTransform& dirLightTrans = keyDirLight->GetTransform();
	// dirLightTrans.SetLocalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);

	Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);

	gameMgrCmp->GameEntities.PushBack(sponza);
}

void GameManagerSystem::CreateSponzaSample(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 40_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(300.0f, 0.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));

	scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));

	Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sponza, "Models/Sponza/sponza_lion.obj", eResourceSource::GAME);
	gameMgrCmp->GameEntities.PushBack(sponza);

	Vector lightPosition = Vector(50.0f, 0.0f, 50.0f);
	Entity* lightEntity = CreatePointLight(scene, lightPosition, 100.0f);
	gameMgrCmp->LightsStartPositions.PushBack(lightPosition);
	gameMgrCmp->PointLightEntities.PushBack(lightEntity);
}

void GameManagerSystem::Update(Scene* scene)
{
	UpdateLights(scene);

	// UpdateModel(scene);

	Vector offset = Vector(800.0f, 0.0f, 0.0f);
	DebugDrawSystem::DrawLine(scene, offset, offset + Vector::UNIT_Y * 1000.0f, Color::RED);
	DebugDrawSystem::DrawBox(scene, offset + Vector(-100.0f, 0.0f, -100.0f), offset + Vector(100.0f, 200.0f, 100.0f), Color::RED);

	// UpdatePostProcess(scene);
}

void GameManagerSystem::UpdatePostProcess(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	time *= 0.1f;
	float exposure = 0.25f * floor(10.0f*4.0f * (time - floor(time)));
	gameMgrCmp->PostCmp->Exposure = exposure;
	gConsole.LogInfo("GameManagerSystem::Update exposure: {}", gameMgrCmp->PostCmp->Exposure);
}

void GameManagerSystem::Deinit(Scene* scene)
{
	gConsole.LogInfo("GameManagerSystem::Deinit");
}

void GameManagerSystem::CreatePointLights(Scene* scene, int quota)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	gameMgrCmp->LightsStartPositions.Reserve(quota);

	for (int i = 0; i < quota; ++i)
	{
		Vector position = Vector(RandomRange(-1.0f, 1.0f)*1000.0f, RandomRange(0.0f, 800.0f), RandomRange(-1.0f, 1.0f)*500.0f);
		float rangeRnd = pow(RandomRange(0.0f, 1.0f), 8.0f);
		float range = 100.0f + rangeRnd * 2000.0f;
		Entity* lightEntity = CreatePointLight(scene, position, range);

		gameMgrCmp->LightsStartPositions.PushBack(position);
		gameMgrCmp->PointLightEntities.PushBack(lightEntity);

		// gConsole.LogInfo("GameManagerSystem::CreateTestLights #{}: Positon: {}, Radius: {}",
		// 	i, position, Range);
	}

	for (size_t i = 0; i < gameMgrCmp->PointLightEntities.GetSize(); ++i)
	{
		gameMgrCmp->GameEntities.PushBack(gameMgrCmp->PointLightEntities[i]);
	}
}

void GameManagerSystem::UpdateLights(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	for (size_t i = 0; i < gameMgrCmp->LightsStartPositions.GetSize(); ++i)
	{
		Entity* pointLight = gameMgrCmp->PointLightEntities[i];
		Vector startPosition = gameMgrCmp->LightsStartPositions[i];
		Vector offset = Vector::UNIT_Y * Sin(2.0_rad * time + 1.0_rad * startPosition.X) * 100.0f;
		EntityTransform& pointLightTrans = pointLight->GetTransform();
		pointLightTrans.SetGlobalTranslation(startPosition + offset);
	}
}

void GameManagerSystem::UpdateModel(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	if (gameMgrCmp->Model)
	{
		EntityTransform& modelTrans = gameMgrCmp->Model->GetTransform();
		modelTrans.SetLocalRotation(Quaternion(Vector::UNIT_Y, 10.0_deg * time));
	}
}

Entity* GameManagerSystem::CreatePointLight(Scene* scene, Vector& position, float Range)
{
	Color lightColor = Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f));

	Entity* pointLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	PointLightComponent* pointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(scene, pointLight, lightColor, 100.0f * 1000.0f, Range);
	MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, pointLight, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	meshCmp->SetMaterial(0, Material(Color(0.0f, 0.0f, 0.0f, 0.0f), lightColor, 0.0f, 1.0f, 0.5f));
	EntityTransform& pointLightTrans = pointLight->GetTransform();
	pointLightTrans.SetGlobalScale(Vector::ONE * 5.0f);
	pointLightTrans.SetGlobalTranslation(position);

	return pointLight;
}

void GameManagerSystem::CreateSpotLight(Scene* scene, float Range)
{
	Vector spotLightPos = Vector(50.0f, 50.0f, 0.0f);
	Color lightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 0.5f), RandomRange(0.0f, 0.2f));
	Quaternion spotLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	// float pointLightRange = 100.0f;
	Entity* spotLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<SpotLightComponent>(scene, spotLight, lightColor, 1.0f, Range, 5.0f, 17.0f);
	EntityTransform& spotLightTrans = spotLight->GetTransform();
	spotLightTrans.SetLocalTranslation(spotLightPos);
	spotLightTrans.SetLocalRotation(spotLightRot);

	Entity* spotLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, spotLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* spotLightMesh = scene->GetComponent<MeshRenderingComponent>(spotLightDebugSource);
	spotLightMesh->SetMaterial(0, Material(lightColor, lightColor, 1.0f, 1.0f, 0.5f));
	EntityTransform& spotLightDebugSourceTrans = spotLightDebugSource->GetTransform();
	spotLightDebugSource->SetParent(spotLight);
	spotLightDebugSourceTrans.SetLocalScale(2.0f);
	spotLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
}

void GameManagerSystem::SpawnShaderball(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* shaderball = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& shaderballTrans = shaderball->GetTransform();
	shaderballTrans.SetLocalTranslation(Vector(0.0f, 5.0f, 0.0f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, shaderball, "Models/shaderball/PolyEngine_shaderball.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ballMesh = scene->GetComponent<MeshRenderingComponent>(shaderball);
	ballMesh->SetMaterial(0, Material(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), 1.0f, 1.0f, 0.5f));
	ballMesh->SetMaterial(1, Material(Color(1.0f, 1.0f, 1.0f), Color(0.4f, 0.4f, 0.4f), 1.0f, 1.0f, 0.5f));
	shaderballTrans.SetLocalScale(0.1f);
	gameMgrCmp->GameEntities.PushBack(shaderball);
}