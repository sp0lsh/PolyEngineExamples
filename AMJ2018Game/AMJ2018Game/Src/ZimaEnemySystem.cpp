#include "ZimaEnemySystem.hpp"
#include "ZimaEnemyComponent.hpp"
#include "ZimaGunComponent.hpp"

void ZimaEnemySystem::Update(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());

	for (auto cmp : world->IterateComponents<ZimaEnemyComponent>())
	{
		ZimaEnemyComponent* enemyCmp = std::get<ZimaEnemyComponent*>(cmp);
		if (enemyCmp->Type == EnemyType::SinusRandom || enemyCmp->Type == EnemyType::Shooting)
		{
			EntityTransform& transform = enemyCmp->GetOwner()->GetTransform();

			Vector deltaTransform = MovementSystem::GetGlobalForward(transform);
			deltaTransform += MovementSystem::GetGlobalRight(transform) * std::sin(time) * 3.f;

			transform.SetGlobalTranslation(transform.GetGlobalTranslation() + deltaTransform * deltaTime * enemyCmp->Speed);
		}
		if (enemyCmp->Type == EnemyType::Shooting)
		{
			ZimaGunComponent* gunCmp = enemyCmp->GetOwner()->GetComponent<ZimaGunComponent>();
			gunCmp->bSpawnBullet = true;
		}

		// health
		enemyCmp->CurrentHealth -= enemyCmp->DamageToBeDealt;
		enemyCmp->DamageToBeDealt = 0.f;
		if (enemyCmp->CurrentHealth <= 0.f)
		{
			DeferredTaskSystem::DestroyEntity(world, enemyCmp->GetOwner());
		}
	}
}