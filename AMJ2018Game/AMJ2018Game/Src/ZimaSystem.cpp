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
#include <Time/TimeSystem.hpp>
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
	DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, gameCmp->Player.Get(), Vector(), Dynarray<Quaternion>{Quaternion()});
	gameCmp->Player->GetComponent<ZimaGunComponent>()->TimeSinceLastBullet = 1000.f;
	gameCmp->Player->GetComponent<ZimaGunComponent>()->TimeSinceLastBullet = 1000.f;
	gameCmp->Entities.PushBack(gameCmp->Player);
	EntityTransform& playerTransform = gameCmp->Player->GetTransform();
	playerTransform.SetGlobalRotation(Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-90.f)));
	playerTransform.SetGlobalScale(Vector(3.f, 3.f, 3.f));
}

void ZimaSystem::UpdateParticles(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	// float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	ZimaWorldComponent* gameCmp = world->GetWorldComponent<ZimaWorldComponent>();

	Vector translation = Vector::UNIT_Y * (0.2f*Abs(Sin(1.0_rad*3.1415f *time + 0.5_rad)));
	Vector scale = Vector::ONE * (1.0f + 0.1f*(0.5f + 0.5f*Sin(2.0_rad*3.1415f *time + 0.5_rad)));
	Quaternion rotation = Quaternion(Vector::UNIT_Y, 0.5_rad * time);

	//if (gameCmp->particleHeart)
	//{
	//	gameCmp->particleHeart->GetTransform().SetGlobalTranslation(translation);
	//	gameCmp->particleHeart->GetTransform().SetGlobalScale(scale);
	//	gameCmp->particleHeart->GetTransform().SetGlobalRotation(rotation);
	//}

	//if (gameCmp->particleHeartImpact0)
	//{
	//	gameCmp->particleHeartImpact0->GetTransform().SetGlobalTranslation(translation);
	//	gameCmp->particleHeartImpact0->GetTransform().SetGlobalScale(scale);
	//	gameCmp->particleHeartImpact0->GetTransform().SetGlobalRotation(rotation);
	//}

	/*if (gameCmp->particleHeartImpact1)
	{
		gameCmp->particleHeartImpact1->GetTransform().SetGlobalRotation(rotation);
	}
*/
//if (gameCmp->particleHeartImpact2)
//{
//	gameCmp->particleHeartImpact2->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * 0.2f);
//	gameCmp->particleHeartImpact2->GetTransform().SetGlobalRotation(rotation);
//}

	if (gameCmp->particleLocalSpace)
	{
		gameCmp->particleLocalSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time), 0.0f, Sin(100.0_deg * time)) * 6.0f);
	}

	//if (gameCmp->particleWorldSpace)
	//{
	//	gameCmp->particleWorldSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time + 180.0_deg), 0.0f, Sin(100.0_deg * time + 180.0_deg)) * 6.0f);
	//}
}


ParticleComponent* ZimaSystem::SpawnEmitterLocalSpace(World* world, Vector pos)
{
	ZimaWorldComponent* gameMgrCmp = world->GetWorldComponent<ZimaWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	gameMgrCmp->Entities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	gameMgrCmp->particleLocalSpace = particleCmp;

	return particleCmp;
}


void ZimaSystem::Update(World* world)
{
	UpdateParticles(world);

	static float playerCollision = 2.f;
	static float bulletCollision = 1.f;
	static float enemyCollision = 15.f;

	ZimaWorldComponent* gameCmp = world->GetWorldComponent<ZimaWorldComponent>();
	for (auto enemy : gameCmp->Enemies)
	{
		if (enemy.Get())
		{
			for (auto bullet : gameCmp->Bullets)
			{
				if (bullet.Get() && bullet.Get()->GetComponent<ZimaBulletComponent>()->Instigator == gameCmp->Player.Get())
				{
					if (IsColliding(bullet.Get(), bulletCollision, enemy.Get(), enemyCollision))
					{
						enemy.Get()->GetComponent<ZimaEnemyComponent>()->DamageToBeDealt = bullet.Get()->GetComponent<ZimaBulletComponent>()->Damage;
						//ZimaSystem::SpawnEmitterLocalSpace(world, bullet.Get()->GetTransform().GetGlobalTranslation());
						DeferredTaskSystem::DestroyEntity(world, bullet.Get());
						bullet.Get()->GetComponent<ZimaBulletComponent>()->bDead = true;
					}
				}
			}

			if (gameCmp->Player.Get())
			{
				if (IsColliding(gameCmp->Player.Get(), playerCollision, enemy.Get(), enemyCollision))
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
			if (IsColliding(bullet.Get(), bulletCollision, gameCmp->Player.Get(), playerCollision))
			{
				//ZimaSystem::SpawnEmitterLocalSpace(world, bullet.Get()->GetTransform().GetGlobalTranslation());
				gameCmp->PlayerHealth -= bullet.Get()->GetComponent<ZimaBulletComponent>()->Damage;
				DeferredTaskSystem::DestroyEntity(world, bullet.Get());
				bullet.Get()->GetComponent<ZimaBulletComponent>()->bDead = true;
			}
		}
	}

	if (gameCmp->PlayerHealth <= 0.f)
	{
		gConsole.LogInfo("PlayerDead");
		TimeSystem::SetTimerMultiplier(world, 0, 0.1);

		world->GetComponent<PostprocessSettingsComponent>(gameCmp->Camera.Get())->Exposure -= 0.1f;

	}

	if (gameCmp->bScreenShake)
	{

	}
}

void ZimaSystem::CreateCamera(World* world)
{
	gConsole.LogInfo("ZimaSystem::CreateCamera");

	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = gameMgrCmp->Camera.Get();
	if (!camera)
	{
		camera = DeferredTaskSystem::SpawnEntityImmediate(world);
		DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, camera, 35_deg, 1.0f, 5000.f);
		gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, camera);
		gameMgrCmp->Camera = camera;
	}
	world->GetWorldComponent<ZimaWorldComponent>()->Camera = camera;

	//DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp->Exposure = 3.0f;

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-50.0f, 580.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -90.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(camera));
	//world->GetComponent<CameraComponent>(camera)->SetRenderingMode(eRenderingModeType::IMMEDIATE_DEBUG);


	Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, keyDirLight, Color(1.0f, 1.0f, 0.90f), 5.0f);
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



