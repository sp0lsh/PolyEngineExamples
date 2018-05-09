#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <ResourceManager.hpp>
#include <DeferredTaskSystem.hpp>
#include <MeshRenderingComponent.hpp>
#include <CameraComponent.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <LightSourceComponent.hpp>
#include <PostprocessSettingsComponent.hpp>
#include <ViewportWorldComponent.hpp>
#include <TimeWorldComponent.hpp>
#include <SkyboxWorldComponent.hpp>
#include <InputWorldComponent.hpp>

using namespace Poly;

void GameManagerSystem::CreateScene(World* world)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	srand(42);

	CreateSponzaSample(world);
}

void GameManagerSystem::CreateSponza(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 90_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));

	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	Entity* Sponza = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);
	GameMgrCmp->GameEntities.PushBack(Sponza);

	CreatePointLights(world, 400);
}

void GameManagerSystem::CreateSponzaSample(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 40_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(300.0f, 0.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));

	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	Entity* Sponza = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Sponza, "Models/Sponza/sponza_lion.obj", eResourceSource::GAME);
	GameMgrCmp->GameEntities.PushBack(Sponza);

	Vector LightPosition = Vector(50.0f, 0.0f, 50.0f);
	Entity* LightEntity = CreatePointLight(world, LightPosition, 10000.f);
	GameMgrCmp->LightsStartPositions.PushBack(LightPosition);
	GameMgrCmp->PointLightEntities.PushBack(LightEntity);
}

void GameManagerSystem::Update(World* world)
{
	float Time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	UpdateLights(world);
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Cleanup");
}

void GameManagerSystem::CreatePointLights(World* world, int quota)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	GameMgrCmp->LightsStartPositions.Reserve(quota);

	for (int i = 0; i < quota; ++i)
	{
		float Range = Random(50.0f, 100.0f) * 1000.0f;
		Vector position = Vector(Random(-1.0f, 1.0f)*1000.0f, Random(0.0f, 800.0f), Random(-1.0f, 1.0f)*500.0f);
		Entity* lightEntity = CreatePointLight(world, position, Range);

		GameMgrCmp->LightsStartPositions.PushBack(position);
		GameMgrCmp->PointLightEntities.PushBack(lightEntity);

		// gConsole.LogInfo("GameManagerSystem::CreateTestLights #{}: Positon: {}, Radius: {}",
		// 	i, position, Range);
	}

	for (int i = 0; i < GameMgrCmp->PointLightEntities.GetSize(); ++i)
	{
		GameMgrCmp->GameEntities.PushBack(GameMgrCmp->PointLightEntities[i]);
	}
}

void GameManagerSystem::UpdateLights(World* world)
{
	float Time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (int i = 0; i < GameMgrCmp->LightsStartPositions.GetSize(); ++i)
	{
		Entity* PointLight = GameMgrCmp->PointLightEntities[i];
		Vector StartPosition = GameMgrCmp->LightsStartPositions[i];
		Vector Offset = Vector::UNIT_Y * Sin(2.0_rad * Time + 1.0_rad * StartPosition.X) * 100.0f;
		EntityTransform& PointLightTrans = PointLight->GetTransform();
		PointLightTrans.SetGlobalTranslation(StartPosition + Offset);
	}
}

Entity* GameManagerSystem::CreatePointLight(World* world, Vector& position, float Range)
{
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(Random(0.0f, 1.0f), Random(0.0, 0.5f), Random(0.0f, 0.2f));

	Entity* PointLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	PointLightComponent* PointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, PointLight, LightColor, 200.0f, Range);
	EntityTransform& PointLightTrans = PointLight->GetTransform();
	PointLightTrans.SetGlobalTranslation(position);

	return PointLight;
}

void GameManagerSystem::CreateSpotLight(World* world, float Range)
{
	Vector SpotLightPos = Vector(50.0f, 50.0f, 0.0f);
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(Random(0.0f, 1.0f), Random(0.0, 0.5f), Random(0.0f, 0.2f));
	Quaternion SpotLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	// float PointLightRange = 100.0f;
	Entity* SpotLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<SpotLightComponent>(world, SpotLight, LightColor, 1.0f, Range, 5.0f, 17.0f);
	EntityTransform& SpotLightTrans = SpotLight->GetTransform();
	SpotLightTrans.SetLocalTranslation(SpotLightPos);
	SpotLightTrans.SetLocalRotation(SpotLightRot);

	Entity* SpotLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, SpotLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* SpotLightMesh = world->GetComponent<MeshRenderingComponent>(SpotLightDebugSource);
	SpotLightMesh->SetShadingModel(eShadingModel::UNLIT);
	SpotLightMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& SpotLightDebugSourceTrans = SpotLightDebugSource->GetTransform();
	SpotLightDebugSource->SetParent(SpotLight);
	SpotLightDebugSourceTrans.SetLocalScale(2.0f);
	SpotLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
}

float GameManagerSystem::Random()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float GameManagerSystem::Random(float min, float max)
{
	float rnd = Random();
	return Lerp(min, max, rnd);
}
