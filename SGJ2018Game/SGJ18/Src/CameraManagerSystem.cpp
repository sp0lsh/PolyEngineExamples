#include "Game.hpp"
#include "CameraManagerSystem.hpp"
#include "GameManagerWorldComponent.hpp"
#include "Input/InputWorldComponent.hpp" //for now
#include "GameCameraComponent.hpp"

using namespace Poly;
using namespace SGJJam;

RTTI_DEFINE_COMPONENT(GameCameraComponent);

void CameraManagerSystem::SwitchCameraCyclic(Scene* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	CameraComponent* newCam = nullptr;

	gameMgrCmp->CurrentCameraIndex = (gameMgrCmp->CurrentCameraIndex + 1) % gameMgrCmp->Cameras.GetSize();
	auto& newCamera = gameMgrCmp->Cameras[gameMgrCmp->CurrentCameraIndex];
	gameMgrCmp->ActiveCamera = newCamera->GetOwner();
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, newCamera);
}

void CameraManagerSystem::Update(Scene* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	for (auto[gameCamera, camera] : world->IterateComponents<GameCameraComponent, CameraComponent>())
	{
		//Init
		if (gameCamera->GetFlags().IsSet(eComponentBaseFlags::NEWLY_CREATED))
		{
			gameMgrCmp->Cameras.PushBack(camera);

			switch (gameCamera->Policy)
			{
				case eGameCameraPolicy::FREE:
					DeferredTaskSystem::AddComponent<FreeFloatMovementComponent>(world, camera->GetOwner(), 15.0f, 0.01f, 0.1f);
					break;
				case eGameCameraPolicy::FOLLOW:
				{
					gameCamera->GetOwner()->SetParent(gameCamera->Target.Get());
					gameCamera->GetOwner()->GetTransform().SetLocalTranslation(Vector(-1, 0, 0.5).Normalize() * gameCamera->TargetDistance * 0.5f);
					gameCamera->GetOwner()->GetTransform().SetLocalRotation(EulerAngles(75_deg, 0_deg, -90_deg));
					gameCamera->GetOwner()->GetTransform().SetLocalScale(Vector::ONE);
					break;
				}
				case eGameCameraPolicy::LOOK_AT:
					gameCamera->GetOwner()->GetTransform().SetLocalTranslation(Vector(0, gameCamera->TargetDistance,0));
					break;
				case eGameCameraPolicy::ORBIT:
				{
					Entity* ent = DeferredTaskSystem::SpawnEntityImmediate(world);
					ent->SetParent(gameCamera->Target.Get());
					ent->GetTransform().SetLocalTranslation(Vector::ZERO);
					ent->GetTransform().SetLocalRotation(Quaternion::IDENTITY);
					ent->GetTransform().SetLocalScale(Vector::ONE);
					gameCamera->GetOwner()->SetParent(ent);
					gameCamera->GetOwner()->GetTransform().SetLocalTranslation(Vector(-1, 0, 1).Normalize() * gameCamera->TargetDistance);
					gameCamera->GetOwner()->GetTransform().SetLocalRotation(EulerAngles(60_deg, 0_deg, -90_deg));
					gameCamera->GetOwner()->GetTransform().SetLocalScale(Vector::ONE);
					break;
				}
				default:
					ASSERTE(false, "Invalid value!");
			}
		}

		// Update
		switch (gameCamera->Policy)
		{
		case eGameCameraPolicy::LOOK_AT:
		{
			if (gameCamera->GetTransform().GetGlobalTranslation() == gameCamera->Target->GetTransform().GetGlobalTranslation())
				break;

			Quaternion lookAtRot = Quaternion::LookAt(
				gameCamera->GetTransform().GetGlobalTranslation(),
				gameCamera->Target->GetTransform().GetGlobalTranslation());
			lookAtRot.Normalize();
			gameCamera->GetTransform().SetGlobalRotation(lookAtRot);
			break;
		}
		case eGameCameraPolicy::ORBIT:
		{
			if (gameCamera->GetTransform().GetGlobalTranslation() == gameCamera->Target->GetTransform().GetGlobalTranslation())
				break;

			Entity* parent = gameCamera->GetOwner()->GetParent();
			float delta = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

			Quaternion lrot = parent->GetTransform().GetLocalRotation();
			parent->GetTransform().SetLocalRotation(lrot * Quaternion(Vector::UNIT_Z, 20_deg * delta));

			break;
		}
		default:
			break;
		}
	}

	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	if (inputCmp->IsClicked(eKey::SPACE))
	{
		SwitchCameraCyclic(world);
	}
}

void CameraManagerSystem::SwitchCameraTo(Scene* world, size_t ind)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, gameMgrCmp->Cameras[ind]);
	SoundSystem::SetListenerPosition(gameMgrCmp->Cameras[ind]->GetTransform().GetGlobalTranslation());
}
