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
	gConsole.LogInfo("ZimaSystem::CreateScene");

	DeferredTaskSystem::AddWorldComponentImmediate<ZimaWorldComponent>(world);
	ZimaWorldComponent* gameCmp = world->GetWorldComponent<ZimaWorldComponent>();

	gameCmp->Player = CreateActor(world, "Models/Drone/OBJ/Drone_00.obj");
	DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, gameCmp->Player.Get(), Vector(), Quaternion());
	gameCmp->Entities.PushBack(gameCmp->Player);
	EntityTransform& playerTransform = gameCmp->Player->GetTransform();
	playerTransform.SetGlobalRotation(Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-90.f)));
	playerTransform.SetGlobalScale(Vector(3.f, 3.f, 3.f));

	Dynarray<Vector> enemyTransforms
	{
		Vector(100.f, 0.f, 200.f),
			Vector(100.f, 0.f,100.f),
			Vector(100.f, 0.f, -100.f),
			Vector(100.f, 0.f, -200.f)
	};

	for (int i = 0; i < enemyTransforms.GetSize(); i++)
	{
		Entity* actor = CreateActor(world, "Models/Primitives/Sphere_HighPoly.obj");
		gameCmp->Entities.PushBack(actor);
		gameCmp->Enemies.PushBack(actor);
		DeferredTaskSystem::AddComponentImmediate<ZimaEnemyComponent>(world, actor, 20.f, 100.f);

		EntityTransform& transform = actor->GetTransform();
		transform.SetGlobalTranslation(enemyTransforms[i]);
		transform.SetGlobalScale(Vector(6.f, 6.f, 6.f));
		transform.SetGlobalRotation(Quaternion(Vector::UNIT_Y, Angle::FromDegrees(90.f)));
	}
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
				if (bullet.Get())
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
					gConsole.LogInfo("ZimaSystem::Collision");
					gameCmp->PlayerHealth -= enemy.Get()->GetComponent<ZimaEnemyComponent>()->DamageOnHit;

					DeferredTaskSystem::DestroyEntity(world, enemy.Get());
				}
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

//
//void GameManagerSystem::Update(World* world)
//{
//	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
//	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
//
//	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
//	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
//
//	UpdateCamera(world);
//
//	UpdateParticles(world);
//
//	if (inputCmp->IsReleased(eKey::RETURN))
//	{
//		GameMgrCmp->SetIsPaused(!GameMgrCmp->GetIsPaused());
//	}
//
//	if (GameMgrCmp->GetIsPaused())
//	{
//		return;
//	}
//
//	if (GameMgrCmp->GetNeedRestart())
//	{
//		RestartGame(world);
//	}
//
//	UpdatePlayer(world);
//
//	UpdateEnemies(world);
//
//	UpdateCollision(world);
//
//	UpdateGameplay(world);
//
//	UpdatePostEffect(world);
//}
//
//void GameManagerSystem::UpdateCollision(World* world)
//{
//	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
//	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
//	PostprocessSettingsComponent* PostCmp = GameMgrCmp->Camera->GetComponent<PostprocessSettingsComponent>();
//
//	for (SafePtr<Entity> Bomb : GameMgrCmp->BombEntities)
//	{
//		if (IsColliding(GameMgrCmp->PlayerShipCollision.Get(), 1.0f, Bomb.Get(), 1.0f))
//		{
//			Vector ExplosionPos = GameMgrCmp->PlayerShipCollision.Get()->GetTransform().GetGlobalTranslation();
//			SpawnExplosionEmitterInWS(world, nullptr, ExplosionPos);
//			SpawnExplosionEmitterInWS2(world, nullptr, ExplosionPos - Vector(0.01f, 0.0f, 0.01f));
//			SpawnExplosionEmitterInWS3(world, nullptr, ExplosionPos - Vector(0.02f, 0.0f, 0.02f));
//			SpawnExplosionEmitterInWS4(world, nullptr, ExplosionPos + Vector(0.02f, 0.0f, 0.02f));
//
//			GameMgrCmp->SetIsPaused(true);
//			GameMgrCmp->SetNeedRestart(true);
//			PostCmp->TimeOfDeath = time;
//
//			// DeferredTaskSystem::DestroyEntityImmediate(world, Bomb.Get());
//		}
//	}
//
//	for (SafePtr<Entity> Bomb : GameMgrCmp->BombEntities)
//	{
//		for (int i = 0; i < GameMgrCmp->EnemyShipCollision.GetSize(); ++i)
//		{
//			if (GameMgrCmp->EnemyShipCollision[i] == nullptr)
//			{
//				continue;
//			}
//
//			Entity* EnemyCollision = GameMgrCmp->EnemyShipCollision[i].Get();
//
//			if (IsColliding(EnemyCollision, 1.0f, Bomb.Get(), 1.0f))
//			{
//				Vector ExplosionPos = EnemyCollision->GetTransform().GetGlobalTranslation();
//				SpawnExplosionEmitterInWS(world, nullptr, ExplosionPos);
//				SpawnExplosionEmitterInWS2(world, nullptr, ExplosionPos - Vector(0.01f, 0.0f, 0.01f));
//				SpawnExplosionEmitterInWS3(world, nullptr, ExplosionPos - Vector(0.02f, 0.0f, 0.02f));
//				SpawnExplosionEmitterInWS4(world, nullptr, ExplosionPos + Vector(0.02f, 0.0f, 0.02f));
//
//				PostCmp->TimeOfAction = time;
//
//				// DeferredTaskSystem::DestroyEntityImmediate(world, Bomb.Get());
//				// DeferredTaskSystem::DestroyEntityImmediate(world, EnemyCollision);
//				GameMgrCmp->EnemyShipCollision[i] = nullptr;
//			}
//		}
//	}
//
//	for (int i = 0; i < GameMgrCmp->EnemyShipCollision.GetSize(); ++i)
//	{
//		if (GameMgrCmp->EnemyShipCollision[i] == nullptr)
//		{
//			continue;
//		}
//
//		Entity* EnemyCollision = GameMgrCmp->EnemyShipCollision[i].Get();
//		Entity* PlayerCollision = GameMgrCmp->PlayerShipCollision.Get();
//
//		if (IsColliding(EnemyCollision, 1.0f, PlayerCollision, 1.0f))
//		{
//			Vector ExplosionPos = EnemyCollision->GetTransform().GetGlobalTranslation();
//			SpawnExplosionEmitterInWS(world, nullptr, ExplosionPos);
//			SpawnExplosionEmitterInWS2(world, nullptr, ExplosionPos - Vector(0.01f, 0.0f, 0.01f));
//			SpawnExplosionEmitterInWS3(world, nullptr, ExplosionPos - Vector(0.02f, 0.0f, 0.02f));
//			SpawnExplosionEmitterInWS4(world, nullptr, ExplosionPos + Vector(0.02f, 0.0f, 0.02f));
//
//			GameMgrCmp->SetIsPaused(true);
//			GameMgrCmp->SetNeedRestart(true);
//
//			PostCmp->TimeOfDeath = time;
//		}
//	}
//
//	for (int i = 0; i < GameMgrCmp->PlayerTorpedos.GetSize(); ++i)
//	{
//		if (GameMgrCmp->PlayerTorpedos[i] == nullptr)
//		{
//			continue;
//		}
//
//		Entity* PlayerTorpedo = GameMgrCmp->PlayerTorpedos[i].Get();
//
//		for (int j = 0; j < GameMgrCmp->EnemyShipCollision.GetSize(); ++j)
//		{
//			if (GameMgrCmp->EnemyShipCollision[j] == nullptr)
//			{
//				continue;
//			}
//
//			Entity* EnemyCollision = GameMgrCmp->EnemyShipCollision[j].Get();
//
//			if (IsColliding(EnemyCollision, 1.0f, PlayerTorpedo, 1.0f))
//			{
//				Vector ExplosionPos = EnemyCollision->GetTransform().GetGlobalTranslation();
//				SpawnExplosionEmitterInWS(world, nullptr, ExplosionPos);
//				SpawnExplosionEmitterInWS2(world, nullptr, ExplosionPos - Vector(0.01f, 0.0f, 0.01f));
//				SpawnExplosionEmitterInWS3(world, nullptr, ExplosionPos - Vector(0.02f, 0.0f, 0.02f));
//				SpawnExplosionEmitterInWS4(world, nullptr, ExplosionPos + Vector(0.02f, 0.0f, 0.02f));
//
//				PostCmp->TimeOfAction = time;
//
//				// DeferredTaskSystem::DestroyEntityImmediate(world, EnemyCollision);
//				// DeferredTaskSystem::DestroyEntityImmediate(world, PlayerTorpedo);
//				GameMgrCmp->EnemyShipCollision[j] = nullptr;
//				GameMgrCmp->PlayerTorpedos[i] = nullptr;
//			}
//		}
//	}
//
//	for (int i = 0; i < GameMgrCmp->EnemyTorpedos.GetSize(); ++i)
//	{
//		if (GameMgrCmp->EnemyTorpedos[i] == nullptr)
//		{
//			continue;
//		}
//
//		Entity* EnemyTorpedo = GameMgrCmp->EnemyTorpedos[i].Get();
//		Entity* PlayerCollision = GameMgrCmp->PlayerShipCollision.Get();
//
//		if (IsColliding(PlayerCollision, 1.0f, EnemyTorpedo, 1.0f))
//		{
//			Vector ExplosionPos = PlayerCollision->GetTransform().GetGlobalTranslation();
//			SpawnExplosionEmitterInWS(world, nullptr, ExplosionPos);
//			SpawnExplosionEmitterInWS2(world, nullptr, ExplosionPos - Vector(0.01f, 0.0f, 0.01f));
//			SpawnExplosionEmitterInWS3(world, nullptr, ExplosionPos - Vector(0.02f, 0.0f, 0.02f));
//			SpawnExplosionEmitterInWS4(world, nullptr, ExplosionPos + Vector(0.02f, 0.0f, 0.02f));
//
//			// DeferredTaskSystem::DestroyEntityImmediate(world, EnemyCollision);
//			// DeferredTaskSystem::DestroyEntityImmediate(world, PlayerTorpedo);
//
//			GameMgrCmp->SetIsPaused(true);
//			GameMgrCmp->SetNeedRestart(true);
//			PostCmp->TimeOfDeath = time;
//		}
//	}
//
//	if (GameMgrCmp->GetIsBossEngaged())
//	{
//		for (int i = 0; i < GameMgrCmp->BossCollision.GetSize(); ++i)
//		{
//			Entity* SubmarineCollision = GameMgrCmp->BossCollision[i].Get();
//			Entity* PlayerCollision = GameMgrCmp->PlayerShipCollision.Get();
//
//			if (IsColliding(PlayerCollision, 1.0f, SubmarineCollision, 3.0f))
//			{
//				Vector ExplosionPos = PlayerCollision->GetTransform().GetGlobalTranslation();
//				SpawnExplosionEmitterInWS(world, nullptr, ExplosionPos);
//				SpawnExplosionEmitterInWS2(world, nullptr, ExplosionPos - Vector(0.01f, 0.0f, 0.01f));
//				SpawnExplosionEmitterInWS3(world, nullptr, ExplosionPos - Vector(0.02f, 0.0f, 0.02f));
//				SpawnExplosionEmitterInWS4(world, nullptr, ExplosionPos + Vector(0.02f, 0.0f, 0.02f));
//
//				// DeferredTaskSystem::DestroyEntityImmediate(world, EnemyCollision);
//				// DeferredTaskSystem::DestroyEntityImmediate(world, PlayerTorpedo);
//
//				SoundSystem::StopEmitter(world, GameMgrCmp->musicEmmiterEnt);
//
//				Entity* musicEmmiter = DeferredTaskSystem::SpawnEntityImmediate(world);
//				DeferredTaskSystem::AddComponentImmediate<Poly::SoundEmitterComponent>(world, musicEmmiter, "Yellow_Submarine.ogg", eResourceSource::GAME);
//				SoundSystem::SetEmitterFrequency(world, musicEmmiter, 1.0f);
//				SoundSystem::SetEmitterGain(world, musicEmmiter, 1.0f);
//				SoundSystem::PlayEmitter(world, musicEmmiter);
//				SoundSystem::LoopEmitter(world, musicEmmiter);
//				GameMgrCmp->submarineEmmiterEnt = musicEmmiter;
//
//				GameMgrCmp->SetIsPaused(true);
//				GameMgrCmp->SetNeedRestart(true);
//
//				PostCmp->TimeOfDeath = time;
//			}
//		}
//	}
//}
//
//void GameManagerSystem::UpdateGameplay(World * world)
//{
//	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
//	PostprocessSettingsComponent* PostCmp = GameMgrCmp->Camera->GetComponent<PostprocessSettingsComponent>();
//
//	gConsole.LogDebug("GameManagerSystem::UpdateGamplay IsBossEngaged: {}", GameMgrCmp->GetIsBossEngaged());
//
//	if (!GameMgrCmp->GetIsBossEngaged())
//	{
//		int EnemiesDestroyed = 0;
//		for (int i = 0; i < GameMgrCmp->EnemyShipCollision.GetSize(); ++i)
//		{
//			gConsole.LogDebug("GameManagerSystem::UpdateGamplay EnemyId: {}, EnemyDead: {}, Score: {}/{}",
//				i, GameMgrCmp->EnemyShipCollision[i] == nullptr, EnemiesDestroyed, GameMgrCmp->GetEnemiesCount()
//			);
//
//			if (GameMgrCmp->EnemyShipCollision[i] == nullptr)
//			{
//				EnemiesDestroyed++;
//			}
//		}
//
//		if (GameMgrCmp->GetEnemiesCount() <= EnemiesDestroyed)
//		{
//			GameMgrCmp->SetIsBossEngaged(true);
//
//			SpawnBoss(world);
//
//			PostCmp->EnableBoss = true;
//		}
//	}
//}

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



