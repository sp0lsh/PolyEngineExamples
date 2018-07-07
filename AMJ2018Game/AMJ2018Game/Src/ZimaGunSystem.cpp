
#include "ZimaGunSystem.hpp"

#include "ZimaGunComponent.hpp"
#include "ZimaBulletComponent.hpp"
#include "ZimaWorldComponent.hpp"

void ZimaGunSystem::Update(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	for (auto cmp : world->IterateComponents<ZimaGunComponent>())
	{
		ZimaGunComponent* gunCmp = std::get<ZimaGunComponent*>(cmp);
		gunCmp->TimeSinceLastBullet += deltaTime;
		if (gunCmp->bSpawnBullet && gunCmp->TimeSinceLastBullet >= gunCmp->BulletSpawnInterval)
		{
			EntityTransform& gunTransform = gunCmp->GetOwner()->GetTransform();
			SpawnBullet(world, gunTransform.GetGlobalTranslation() + gunCmp->LocalTranslation, gunTransform.GetGlobalRotation() * gunCmp->LocalRotation);
			gunCmp->TimeSinceLastBullet = 0.f;
		}
		gunCmp->bSpawnBullet = false;
	}
}

void ZimaGunSystem::SpawnBullet(World* world, const Vector& translation, const Quaternion& quaternion)
{
	Entity* actor = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(world, actor, eDebugDrawPreset::STATIC);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, actor, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
	DeferredTaskSystem::AddComponentImmediate<ZimaBulletComponent>(world, actor);
	EntityTransform& transform = actor->GetTransform();
	transform.SetGlobalTranslation(translation);
	transform.SetGlobalRotation(quaternion);
	
	world->GetWorldComponent<ZimaWorldComponent>()->Entities.PushBack(actor);

}


