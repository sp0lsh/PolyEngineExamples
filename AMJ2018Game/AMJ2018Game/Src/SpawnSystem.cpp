#include "SpawnSystem.hpp"
#include "SpawnWorldComponent.hpp"
#include "ZimaSystem.hpp"
#include "ZimaWorldComponent.hpp"
#include "ZimaEnemyComponent.hpp"
#include "ZimaGunComponent.hpp"

void SpawnSystem::SpawnEnemies(World* world)
{
	SpawnWorldComponent* spawnCmp = world->GetWorldComponent<SpawnWorldComponent>();
	ZimaWorldComponent* gameCmp = world->GetWorldComponent<ZimaWorldComponent>();
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());


	spawnCmp->WaveIndex++;
	if (spawnCmp->WaveIndex >= spawnCmp->Waves.GetSize())
		return;
	//if (spawnCmp->WaveIndex >= spawnCmp->Waves.GetSize())
	//	spawnCmp->WaveIndex = 0;

	for (int i = 0; i < spawnCmp->Waves[spawnCmp->WaveIndex].enemyTransforms.GetSize(); i++)
	{
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::None)
			continue;

		Entity* actor = ZimaSystem::CreateActor(world, "Models/Drone/OBJ/Drone_00.obj");
		EntityTransform& transform = actor->GetTransform();
		transform.SetGlobalTranslation(spawnCmp->Waves[spawnCmp->WaveIndex].enemyTransforms[i] + spawnCmp->Waves[spawnCmp->WaveIndex].Offset);
		MeshRenderingComponent* mesh = actor->GetComponent<MeshRenderingComponent>();
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::SinusRandom)
		{
			//mesh->SetMaterial(0, Material(Color(0.1, 0.1, 0.1) * 1.f, Color::BLACK, 1.f, 1.f, 0.5f));
			//mesh->SetMaterial(1, Material(Color(0.1, 0.1, 0.1) * 1, Color::BLACK, 1.f, 1.f, 0.5f));
			//mesh->SetMaterial(2, Material(Color(0.1, 0.1, 0.1) * 1, Color::BLACK, 1.f, 1.f, 0.5f));
			//mesh->SetMaterial(3, Material(Color(0.1, 0.1, 0.1) * 1, Color::BLACK, 1.f, 1.f, 0.5f));
			//mesh->SetMaterial(4, Material(Color(0.1,0.1,0.1) * 1, Color::BLACK, 1.f, 1.f, 0.5f));
			//mesh->SetMaterial(5, Material(Color(0.1, 0.1, 0.1) * 1, Color::BLACK, 1.f, 1.f, 0.5f));
			transform.SetGlobalScale(Vector(2.5f, 2.5f, 2.5f));
		}
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::Shooting)
		{
			transform.SetGlobalScale(Vector(3.5f, 3.5f, 3.5f));
		}
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::SinusShooting)
		{
			transform.SetGlobalScale(Vector(4.f, 4.f, 4.f));
		}
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::Wachlarz)
		{
			transform.SetGlobalScale(Vector(5.f, 5.f, 5.f));
		}
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::SinusWachlarz)
		{
			transform.SetGlobalScale(Vector(5.5f, 5.5f, 5.5f));
		}

		transform.SetGlobalRotation(Quaternion(Vector::UNIT_Y, Angle::FromDegrees(90.f)));


		gameCmp->Entities.PushBack(actor);
		gameCmp->Enemies.PushBack(actor);
		DeferredTaskSystem::AddComponentImmediate<ZimaEnemyComponent>(world, actor, spawnCmp->Waves[spawnCmp->WaveIndex].Types[i], 20.f, 100.f, time  /*i*/);
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::SinusShooting || spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::Shooting)
		{
			Dynarray<Quaternion> rots{ Quaternion()/*,
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(30.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-30.f)) */ };
			DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, actor, Vector(), rots, 0.4f);

			/*
						Entity* gun = DeferredTaskSystem::SpawnEntityImmediate(world);
						gun->SetParent(actor);
						EntityTransform& trans = gun->GetTransform();
						trans.SetGlobalTranslation(Vector());
						trans.SetGlobalRotation(Quaternion());
						gameCmp->Entities.PushBack(gun);
						DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, gun, Vector(), Dynarray<Quaternion>{Quaternion()});*/

		}
		if (spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::Wachlarz || spawnCmp->Waves[spawnCmp->WaveIndex].Types[i] == EnemyType::SinusWachlarz)
		{
			Dynarray<Quaternion> rots{ Quaternion(),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(30.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(45.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(15.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(0.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-15.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-45.f)),
				Quaternion(Vector::UNIT_Y, Angle::FromDegrees(-30.f)) };
			DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, actor, Vector(), rots, 1.f);

			/*
			Entity* gun = DeferredTaskSystem::SpawnEntityImmediate(world);
			gun->SetParent(actor);
			EntityTransform& trans = gun->GetTransform();
			trans.SetGlobalTranslation(Vector());
			trans.SetGlobalRotation(Quaternion());
			gameCmp->Entities.PushBack(gun);
			DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, gun, Vector(), Dynarray<Quaternion>{Quaternion()});*/

		}

	}

	spawnCmp->WaveInterval = spawnCmp->Waves[spawnCmp->WaveIndex].WaitAfterWave;

}

void SpawnSystem::Init(World* world)
{
	DeferredTaskSystem::AddWorldComponentImmediate<SpawnWorldComponent>(world);

}

void SpawnSystem::Update(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	SpawnWorldComponent* spawnCmp = world->GetWorldComponent<SpawnWorldComponent>();
	spawnCmp->TimeSinceLastWave += deltaTime;
	if (spawnCmp->TimeSinceLastWave >= spawnCmp->WaveInterval)
	{
		spawnCmp->TimeSinceLastWave = 0;
		SpawnEnemies(world);
	}
}
