
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
			//DebugDrawSystem::DrawSphere(world, gunTransform.GetGlobalTranslation(), 10.f);

			for (auto rot : gunCmp->LocalRotations)
				SpawnBullet(world, gunCmp, gunTransform.GetGlobalTranslation() + gunCmp->LocalTranslation, gunTransform.GetGlobalRotation() * rot);
			gunCmp->TimeSinceLastBullet = 0.f;
		}
		gunCmp->bSpawnBullet = false;
	}
}

void ZimaGunSystem::SpawnBullet(World* world, ZimaGunComponent* gunCmp, const Vector& translation, const Quaternion& quaternion)
{
	Entity* actor = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(world, actor, eDebugDrawPreset::STATIC);
	MeshRenderingComponent* mesh = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, actor, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	mesh->SetMaterial(0, Material(Color::RED * 1500.f, Color::WHITE, 1.f, 0.5f, 0.5f));

	DeferredTaskSystem::AddComponentImmediate<ZimaBulletComponent>(world, actor);
	PointLightComponent* pointLightCmp =
		DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, actor, Color::RED, 1.f, 10.f);
	actor->GetComponent<ZimaBulletComponent>()->Instigator = gunCmp->GetOwner();
	EntityTransform& transform = actor->GetTransform();
	transform.SetGlobalTranslation(translation);
	transform.SetGlobalRotation(quaternion);

	world->GetWorldComponent<ZimaWorldComponent>()->Entities.PushBack(actor);
	world->GetWorldComponent<ZimaWorldComponent>()->Bullets.PushBack(actor);

}


