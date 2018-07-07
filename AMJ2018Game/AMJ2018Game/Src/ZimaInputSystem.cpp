#include "ZimaInputSystem.hpp"

#include "ZimaWorldComponent.hpp"
#include "ZimaGunComponent.hpp"

void ZimaInputSystem::Update(World* world)
{
	float time = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	float speed = 200.f;

	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	auto zimaComp = world->GetWorldComponent<ZimaWorldComponent>();
	if (zimaComp && zimaComp->Player)
	{
		EntityTransform& playerTransform = zimaComp->Player->GetTransform();
		Vector moveDirection;
		if (inputCmp->IsPressed(eKey::KEY_W))
			moveDirection += Vector::UNIT_X;
		else if (inputCmp->IsPressed(eKey::KEY_S))
			moveDirection -= Vector::UNIT_X;

		if (inputCmp->IsPressed(eKey::KEY_A))
			moveDirection -= Vector::UNIT_Z;
		else if (inputCmp->IsPressed(eKey::KEY_D))
			moveDirection += Vector::UNIT_Z;

		if (moveDirection.LengthSquared() > 0)
		{
			
			Vector newPos = playerTransform.GetGlobalTranslation() + moveDirection.GetNormalized() * time * speed;
			float maxX = 70.f;
			float maxZ = 300.f;
			float minX = -200.f;
			float minZ = -300.f;

			if (newPos.X > maxX)
				newPos.X = maxX;
			if (newPos.X < minX)
				newPos.X = minX;
			if (newPos.Z > maxZ)
				newPos.Z = maxZ;
			if (newPos.Z < minZ)
				newPos.Z = minZ;

			playerTransform.SetGlobalTranslation(newPos);
		}

		if (inputCmp->IsPressed(eKey::SPACE))
		{
			auto gun = zimaComp->Player->GetComponent<ZimaGunComponent>();
			if(gun)
				gun->bSpawnBullet = true;
		}

	}
}