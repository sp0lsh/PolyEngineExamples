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

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 90_deg, 1.0f, 1000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetLocalTranslation(Vector(0.0f, 5.0f, 15.0f));
	
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	Entity* Plane = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& planeTrans = Plane->GetTransform();
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Plane, "Models/plane/plane.obj", eResourceSource::GAME);
	MeshRenderingComponent* planeMesh = world->GetComponent<MeshRenderingComponent>(Plane);
	planeMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));
	GameMgrCmp->GameEntities.PushBack(Plane);

	Entity* Monkey = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& monkeyTrans = Monkey->GetTransform();
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Monkey, "Models/plane/monkey2.obj", eResourceSource::GAME);
	MeshRenderingComponent* monkeyMesh = world->GetComponent<MeshRenderingComponent>(Monkey);
	GameMgrCmp->GameEntities.PushBack(Monkey);

	CreateTestLights(world, 1024);
}

void GameManagerSystem::Update(World* world)
{
	float Time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	UpdateTestLights(world);
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Cleanup");
}

void GameManagerSystem::CreateTestLights(World* world, int quota)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	GameMgrCmp->LightsStartPositions.Reserve(quota);

	for (int i = 0; i < quota; ++i)
	{
		float rotationRadius = 100.0f;
		Vector position = Vector(Random(-1.0f, 1.0f)*rotationRadius, 0.0f, Random(-1.0f, 1.0f)*rotationRadius);
		Entity* lightEntity = CreatePointLight(world, position, 10.0f);

		GameMgrCmp->LightsStartPositions.PushBack(position);
		GameMgrCmp->PointLightEntities.PushBack(lightEntity);

		gConsole.LogInfo("GameManagerSystem::CreateTestLights #{}: Positon: {}, Radius: {}",
			i, position, 10.0f);
	}

	for (int i = 0; i < GameMgrCmp->PointLightEntities.GetSize(); ++i)
	{
		GameMgrCmp->GameEntities.PushBack(GameMgrCmp->PointLightEntities[i]);
	}
}

void GameManagerSystem::UpdateTestLights(World* world)
{
	float Time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (int i = 0; i < GameMgrCmp->LightsStartPositions.GetSize(); ++i)
	{
		Entity* PointLight = GameMgrCmp->PointLightEntities[i];
		Vector StartPosition = GameMgrCmp->LightsStartPositions[i];
		
		float t = 2.0f * Time + StartPosition.X;
		float s = Sin(1.0_rad * t);
		float c = Cos(1.0_rad * t);
		Vector Offset = Vector(c, 0.0f, s) * 20.0f;
		EntityTransform& PointLightTrans = PointLight->GetTransform();
		PointLightTrans.SetGlobalTranslation(StartPosition + Offset);
	}
}

Entity* GameManagerSystem::CreatePointLight(World* world, Vector& position, float Range)
{
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(Random(0.0f, 1.0f), Random(0.0, 0.5f), Random(0.0f, 0.2f));

	Entity* PointLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	PointLightComponent* PointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, PointLight, LightColor, 1.0f, Range);
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
