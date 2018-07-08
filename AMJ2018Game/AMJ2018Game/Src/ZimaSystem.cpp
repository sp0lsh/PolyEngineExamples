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
#include "ZimaGunComponent.hpp"
#include "ZimaEnemyComponent.hpp"
#include "ZimaBulletComponent.hpp"



using namespace Poly;

void ZimaSystem::Init(World* world)
{
	gConsole.LogInfo("ZimaSystem::Init");

	DeferredTaskSystem::AddWorldComponentImmediate<ZimaWorldComponent>(world);
	ZimaWorldComponent* gameCmp = world->GetWorldComponent<ZimaWorldComponent>();

	gameCmp->Player = CreateActor(world, "Models/Drone/OBJ/Drone_00.obj");
	DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, gameCmp->Player.Get(), Vector(), Quaternion());
	gameCmp->Entities.PushBack(gameCmp->Player);
	EntityTransform& playerTransform = gameCmp->Player->GetTransform();
	playerTransform.SetGlobalRotation(Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-90.f)));
	playerTransform.SetGlobalScale(Vector(3.f, 3.f, 3.f));
}

void ZimaSystem::Update(World* world)
{
	DebugDrawSystem::DrawSphere(world, Vector(-100.f, 0.f, 100.f), 10.f);

	ZimaWorldComponent* gameCmp = world->GetWorldComponent<ZimaWorldComponent>();
	for (auto enemy : gameCmp->Enemies)
	{
		if (enemy.Get())
		{
			for (auto bullet : gameCmp->Bullets)
			{
				if (bullet.Get() && bullet.Get()->GetComponent<ZimaBulletComponent>()->Instigator == gameCmp->Player.Get())
				{
					if (IsColliding(bullet.Get(), 5.f, enemy.Get(), 10.f))
					{
						enemy.Get()->GetComponent<ZimaEnemyComponent>()->DamageToBeDealt = bullet.Get()->GetComponent<ZimaBulletComponent>()->Damage;
						DeferredTaskSystem::DestroyEntity(world, bullet.Get());
						bullet.Get()->GetComponent<ZimaBulletComponent>()->bDead = true;
					}
				}
			}

			if (gameCmp->Player.Get())
			{
				if (IsColliding(gameCmp->Player.Get(), 10.f, enemy.Get(), 10.f))
				{
					//gConsole.LogInfo("ZimaSystem::Collision");
					gameCmp->PlayerHealth -= enemy.Get()->GetComponent<ZimaEnemyComponent>()->DamageOnHit;

					DeferredTaskSystem::DestroyEntity(world, enemy.Get());
				}
			}
		}
	}
	for (auto bullet : gameCmp->Bullets)
	{
		if (bullet.Get() && bullet.Get()->GetComponent<ZimaBulletComponent>()->Instigator != gameCmp->Player.Get())
		{
			if (IsColliding(bullet.Get(), 5.f, gameCmp->Player.Get(), 10.f))
			{
				gameCmp->PlayerHealth -= bullet.Get()->GetComponent<ZimaBulletComponent>()->Damage;
				DeferredTaskSystem::DestroyEntity(world, bullet.Get());
				bullet.Get()->GetComponent<ZimaBulletComponent>()->bDead = true;
			}
		}
	}




	if (gameCmp->PlayerHealth <= 0.f)
	{
		gConsole.LogInfo("PlayerDead");
	}
}

void ZimaSystem::CreateCamera(World* world)
{	
	gConsole.LogInfo("ZimaSystem::CreateCamera");

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
	world->GetComponent<CameraComponent>(camera)->SetRenderingMode(eRenderingModeType::IMMEDIATE_DEBUG);

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
	ZimaWorldComponent* zimaCmp = world->GetWorldComponent<ZimaWorldComponent>();
	for (auto entit : zimaCmp->Entities)
	{
		DeferredTaskSystem::DestroyEntity(world, entit.Get());
	}
	gConsole.LogInfo("ZimaSystem::Cleanup");
}

void ZimaSystem::ReadQuote(World* world, String path)
{
	gConsole.LogInfo("GameManagerSystem::CreateAnimTrack");
	String animSrc = LoadTextFileRelative(eResourceSource::GAME, path);
	// gConsole.LogInfo("GameManagerSystem::CreateAnimTrack AnimSrc: {}", animSrc);

	Dynarray<Vector> positions;
	Dynarray<Vector> scales;
	Dynarray<Quaternion> rotations;

	bool hasAnimationKey = false;
	bool hasAnimationKeyRotation = false;
	bool hasAnimationKeyScale = false;
	bool hasAnimationKeyPosition = false;
	int rowCounterRotation = 0;
	int rowCounterScale = 0;
	int rowCounterPosition = 0;

	animSrc.Replace("\r\n", "\n");
	animSrc.GetTrimmed();
	Dynarray<String> rows = animSrc.Split('\n');
	for (String row : rows)
	{
		if (row.IsEmpty())
		{
			continue;
		}

		if (row.Contains("AnimationKey"))
		{
			hasAnimationKey = true;
		}

		if (row.Contains("Rotation"))
		{
			hasAnimationKeyRotation = true;
		}
	}
}

bool ZimaSystem::IsColliding(Entity* EntityA, float RadiusA, Entity* EntityB, float RadiusB)
{
	EntityTransform& ShipTrans = EntityA->GetTransform();
	Vector EntityAPosition = ShipTrans.GetGlobalTranslation();
	EntityAPosition.Y = 0.0f;

	EntityTransform& BombTrans = EntityB->GetTransform();
	Vector EntityBPosition = BombTrans.GetGlobalTranslation();
	EntityBPosition.Y = 0.0f;

	float Distance = (EntityAPosition - EntityBPosition).Length();
	// gConsole.LogInfo("GameManagerSystem::CollideWithBomb Distance: {}", Distance );
	return Distance < (RadiusA + RadiusB);
}



