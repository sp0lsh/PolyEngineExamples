#include "ZimaSystem.hpp"

#include "ZimaWorldComponent.hpp"

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
#include "ZimaMovementComponent.hpp"
#include "GameManagerWorldComponent.hpp"


using namespace Poly;

void ZimaSystem::Init(World* world)
{
	gConsole.LogInfo("ZimaSystem::CreateScene");

	DeferredTaskSystem::AddWorldComponentImmediate<ZimaWorldComponent>(world);
	ZimaWorldComponent* zimaCmp = world->GetWorldComponent<ZimaWorldComponent>();

	zimaCmp->Player = CreateActor(world, "Models/Primitives/Sphere_HighPoly.obj");
	zimaCmp->Entities.PushBack(zimaCmp->Player);

	EntityTransform& playerTransform = zimaCmp->Player->GetTransform();
	playerTransform.SetGlobalScale(Vector(30.f, 30.f, 3.f));

	//GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();	

	//srand(42);
	//
	//CreateCamera(world);

	//DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, "HDR/HDR.hdr", eResourceSource::GAME);
	//
	//CreatePBRShpereGrid(world, Vector(0.0f, 0.0f, 0.0f), Color(0.0f, 0.0f, 0.0f, 1.0f));
	//CreatePBRShpereGrid(world, Vector(-300.0f, 0.0f, 0.0f), Color(0.5f, 0.5f, 0.5f, 1.0f));
	//CreatePBRShpereGrid(world, Vector(-600.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));

	//CreateTextUI(world);

	//// CreateTranslucent(world);

	//CreatePointLights(world, 128);

	//// SpawnParticles(world);
}

void ZimaSystem::CreateCamera(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, camera, 35_deg, 1.0f, 5000.f);
	//DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, camera);
	gameMgrCmp->PostCmp->Exposure = 3.0f;

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-50.0f, 580.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -90.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(camera));
	gameMgrCmp->Camera = camera;

	Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, keyDirLight, Color(1.0f, 1.0f, 1.0f), 5.0f);
	keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	gameMgrCmp->GameEntities.PushBack(keyDirLight);

}

Entity* ZimaSystem::CreateActor(World* world, String path)
{
	Entity* actor = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(world, actor, eDebugDrawPreset::STATIC);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, actor, path, eResourceSource::GAME);
	DeferredTaskSystem::AddComponentImmediate<ZimaMovementComponent>(world, actor);
	return actor;
}



void ZimaSystem::Deinit(World* world)
{
	/*ZimaWorldComponent* zimaCmp = world->GetWorldComponent<ZimaWorldComponent>();
	for (auto entit : zimaCmp->Entities)
	{

	}*/
	gConsole.LogInfo("ZimaSystem::Cleanup");
}


// #pragma endregion