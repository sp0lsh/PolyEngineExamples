#include "ZimaInputSystem.hpp"

#include "ZimaWorldComponent.hpp"

void ZimaInputSystem::Update(World* world)
{
	gConsole.LogInfo("ZimaSystem::Update");
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());


	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();

	if (auto zimaComp = world->GetWorldComponent<ZimaWorldComponent>())
		if (zimaComp->Player)
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

			//world->IterateComponents<>()

			if (moveDirection.LengthSquared() > 0)
			{
				Vector newPos = playerTransform.GetGlobalTranslation() + moveDirection * time;
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

				//float moveTime = actorCmp->GetElapsedMoveTime();

				//const float shift1Speed = 10.0f;
				//const float shift2Speed = 15.0f;
				//const float aFactor = 0.1f;
				//const float bFactor = 0.1f;
				//const float cFactor = 0.005f;


				//float shift1 = sin(shift1Speed * (moveTime + deltaTime)) - sin(shift1Speed * moveTime);
				//float shift2 = sin(shift2Speed * (moveTime + deltaTime) + M_PI / 2) - sin(shift2Speed * moveTime + M_PI / 2);

				//shift1 *= aFactor;
				//shift2 *= cFactor;

				//cameraTransform.SetLocalTranslation(cameraTransform.GetLocalTranslation() + Vector(0, shift1, 0));
				//gunTransform.SetLocalTranslation(gunTransform.GetLocalTranslation() + Vector(shift1 * bFactor, shift2, 0));
			}
			//UpdateParticles(world);

			//UpdateLights(world);

			//// UpdateModel(world);

			//Vector offset = Vector(800.0f, 0.0f, 0.0f);
			//DebugDrawSystem::DrawLine(world, offset, offset + Vector::UNIT_Y * 1000.0f, Color::RED);
			//DebugDrawSystem::DrawBox(world, offset + Vector(-100.0f, 0.0f, -100.0f), offset + Vector(100.0f, 200.0f, 100.0f), Color::RED);

			// UpdatePostProcess(world);
		}
}