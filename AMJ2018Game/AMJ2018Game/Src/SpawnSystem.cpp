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

	for (int i = 0; i < spawnCmp->Waves[0].enemyTransforms.GetSize(); i++)
	{
		Entity* actor = ZimaSystem::CreateActor(world, "Models/Primitives/Sphere_HighPoly.obj");
		gameCmp->Entities.PushBack(actor);
		gameCmp->Enemies.PushBack(actor);
		DeferredTaskSystem::AddComponentImmediate<ZimaEnemyComponent>(world, actor, spawnCmp->Waves[0].Types[i], 20.f, 100.f);
		if (spawnCmp->Waves[0].Types[i] == EnemyType::Shooting)
		{
			DeferredTaskSystem::AddComponentImmediate<ZimaGunComponent>(world, actor, Vector(), Quaternion());
			//actor->GetComponent<ZimaGunComponent>()->Instigator
		}
		EntityTransform& transform = actor->GetTransform();
		transform.SetGlobalTranslation(spawnCmp->Waves[0].enemyTransforms[i]);
		transform.SetGlobalScale(Vector(6.f, 6.f, 6.f));
		transform.SetGlobalRotation(Quaternion(Vector::UNIT_Y, Angle::FromDegrees(90.f)));
	}
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
