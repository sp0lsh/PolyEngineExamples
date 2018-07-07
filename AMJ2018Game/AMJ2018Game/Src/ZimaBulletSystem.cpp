#include "ZimaBulletSystem.hpp"
#include "ZimaBulletComponent.hpp"
#include <Movement/MovementSystem.hpp>

void ZimaBulletSystem::Update(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	for (auto cmp : world->IterateComponents<ZimaBulletComponent>())
	{
		ZimaBulletComponent* bulletCmp = std::get<ZimaBulletComponent*>(cmp);
		EntityTransform& transform = bulletCmp->GetOwner()->GetTransform();

		Vector ForwardVector = MovementSystem::GetGlobalForward(transform);
		transform.SetGlobalTranslation(transform.GetGlobalTranslation() + ForwardVector * deltaTime * bulletCmp->Speed);

	}
}