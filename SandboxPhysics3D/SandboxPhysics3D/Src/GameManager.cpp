#include "GameManager.hpp"

#include <DeferredTaskSystem.hpp>
#include <ViewportWorldComponent.hpp>
#include <CameraComponent.hpp>
#include <TransformComponent.hpp>
#include <PostprocessSettingsComponent.hpp>
#include <Rigidbody3DComponent.hpp>
#include <Trigger3DComponent.hpp>
#include <Physics3DShapes.hpp>
#include <Physics3DSystem.hpp>
#include <MeshRenderingComponent.hpp>
#include <ConfigBase.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <LightSourceComponent.hpp>
#include <InputSystem.hpp>
#include <InputWorldComponent.hpp>

#include "GameManagerWorldComponent.hpp"
#include "StatusFlagsComponent.hpp"

using namespace Poly;

void BT::GameManagerSystem::Update(World* world)
{
	GameManagerWorldComponent* gameManager = Poly::gEngine->GetWorld()->GetWorldComponent<GameManagerWorldComponent>();
	InputWorldComponent* input = world->GetWorldComponent<InputWorldComponent>();
	
	if (input->IsClicked(eMouseButton::RIGHT))
		SpawnBullet(world);
	
	for(auto tuple : world->IterateComponents<TransformComponent>())
	{
		TransformComponent* cmp = std::get<TransformComponent*>(tuple);
	
		if (cmp->GetGlobalTranslation().Length() > 100 && cmp->GetOwnerID() != gameManager->Camera)
			DeferredTaskSystem::DestroyEntity(world, cmp->GetOwnerID());
	}

	// initialize physics values
	StatusFlagsComponent* groundStatus = world->GetComponent<StatusFlagsComponent>(gameManager->Ground);
	if (!groundStatus->PhysicsInitialized)
	{
		Rigidbody3DComponent* rigidbody = groundStatus->GetSibling<Rigidbody3DComponent>();

		rigidbody->SetRestitution(0);
		rigidbody->SetFriction(1);
		rigidbody->SetRollingFriction(1);
		rigidbody->UpdatePosition();

		groundStatus->PhysicsInitialized = true;
	}

	for (int i = 0; i < gameManager->Bricks.GetSize(); i++)
	{
		StatusFlagsComponent* brickStatus = world->GetComponent<StatusFlagsComponent>(gameManager->Bricks[i]);
		if (!brickStatus->PhysicsInitialized)
		{
			Rigidbody3DComponent* rigidbody = brickStatus->GetSibling<Rigidbody3DComponent>();

			rigidbody->SetRestitution(0);
			rigidbody->SetFriction(1);
			rigidbody->SetRollingFriction(1);
			rigidbody->UpdatePosition();
			rigidbody->ApplyGravity();
			rigidbody->SetDamping(0.f, 0.f);

			brickStatus->PhysicsInitialized = true;
		}
	}

	for (int i = 0; i < gameManager->Bullets.GetSize(); i++)
	{
		StatusFlagsComponent* bulletStatus = world->GetComponent<StatusFlagsComponent>(gameManager->Bullets[i]);
		Rigidbody3DComponent* rigidbody = bulletStatus->GetSibling<Rigidbody3DComponent>();
		if (!bulletStatus->PhysicsInitialized && rigidbody->IsRegistered())
		{
			TransformComponent* bulletTransform = bulletStatus->GetSibling<TransformComponent>();

			TransformComponent* transform = world->GetComponent<TransformComponent>(gameManager->Camera);
			Vector direction = MovementSystem::GetGlobalForward(transform);

			bulletTransform->SetLocalTranslation(transform->GetGlobalTranslation());

			rigidbody->UpdatePosition();
			rigidbody->ApplyGravity();
			rigidbody->ApplyImpulseToCenter(direction * 100);
			rigidbody->SetRestitution(0.3);
			rigidbody->SetFriction(0.3);
			rigidbody->SetRollingFriction(0.3);

			bulletStatus->PhysicsInitialized = true;
		}
	}
}

void BT::GameManagerSystem::InitializeDemoWorld(World* world)
{
	GameManagerWorldComponent* gameManager = gEngine->GetWorld()->GetWorldComponent<GameManagerWorldComponent>();

	// create camera
	gameManager->Camera = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
	DeferredTaskSystem::AddComponentImmediate<TransformComponent>(gEngine->GetWorld(), gameManager->Camera);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(gEngine->GetWorld(), gameManager->Camera, 25, 0.01);
	DeferredTaskSystem::AddComponentImmediate<Poly::CameraComponent>(gEngine->GetWorld(), gameManager->Camera, 60_deg, 1.0f, 1000.f);
	gEngine->GetWorld()->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, gEngine->GetWorld()->GetComponent<CameraComponent>(gameManager->Camera));
	DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(gEngine->GetWorld(), gameManager->Camera);
	TransformComponent* cameraTransform = gEngine->GetWorld()->GetComponent<TransformComponent>(gameManager->Camera);
	cameraTransform->SetLocalTranslation(Vector(0, 15, 40));
	cameraTransform->SetLocalRotation(Quaternion({ Angle::FromDegrees(-15), Angle::FromDegrees(0), Angle::FromDegrees(0) }));
	gEngine->GetWorld()->GetComponent<PostprocessSettingsComponent>(gameManager->Camera)->UseFgShader = false;
	gEngine->GetWorld()->GetComponent<PostprocessSettingsComponent>(gameManager->Camera)->UseBgShader = false;

	// create light
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.2f, 0.5f, 1.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.5f);
	Quaternion DirLightRot = Quaternion(Vector::UNIT_Y, 80_deg) * Quaternion(Vector::UNIT_X, -80_deg);

	gameManager->DirectionalLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<StatusFlagsComponent>(gEngine->GetWorld(), gameManager->Camera);
	DeferredTaskSystem::AddComponentImmediate<Poly::TransformComponent>(world, gameManager->DirectionalLight);
	DeferredTaskSystem::AddComponentImmediate<Poly::DirectionalLightComponent>(world, gameManager->DirectionalLight, Color(1.0f, 0.9f, 0.8f), 0.8f);
	Poly::TransformComponent* dirLightTrans = world->GetComponent<Poly::TransformComponent>(gameManager->DirectionalLight);
	dirLightTrans->SetLocalRotation(DirLightRot);

	// create ground
	gameManager->Ground = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());

	DeferredTaskSystem::AddComponentImmediate<StatusFlagsComponent>(gEngine->GetWorld(), gameManager->Ground);
	DeferredTaskSystem::AddComponentImmediate<TransformComponent>(gEngine->GetWorld(), gameManager->Ground);
	DeferredTaskSystem::AddComponentImmediate<Trigger3DComponent>(gEngine->GetWorld(), gameManager->Ground, gEngine->GetWorld(), reinterpret_cast<Physics3DShape*>(new Physics3DBoxShape(Vector(20, 20, 0))));
	DeferredTaskSystem::AddComponentImmediate<Rigidbody3DComponent>(gEngine->GetWorld(), gameManager->Ground, gEngine->GetWorld(), eRigidBody3DType::STATIC, 0);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, gameManager->Ground, "Models/Ground.fbx", eResourceSource::GAME);
	
	world->GetComponent<MeshRenderingComponent>(gameManager->Ground)->SetMaterial(0, PhongMaterial(Color(1, 0.5, 0.2), Color(1, 0.5, 0.2), Color(1, 0.5, 0.2), 8.0f));
	world->GetComponent<MeshRenderingComponent>(gameManager->Ground)->SetShadingModel(eShadingModel::LIT);
	
	TransformComponent* groundTransform = gEngine->GetWorld()->GetComponent<TransformComponent>(gameManager->Ground);
	Rigidbody3DComponent* groundRigidbody = gEngine->GetWorld()->GetComponent<Rigidbody3DComponent>(gameManager->Ground);
	groundTransform->SetLocalTranslation(Vector(0, 0, 0));
	groundTransform->SetLocalRotation(Quaternion({ -90_deg, 0_deg, 0_deg }));

	// build the wall
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			UniqueID brick = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
			gameManager->Bricks.PushBack(brick);

			DeferredTaskSystem::AddComponentImmediate<StatusFlagsComponent>(gEngine->GetWorld(), brick);
			DeferredTaskSystem::AddComponentImmediate<TransformComponent>(gEngine->GetWorld(), brick);
			DeferredTaskSystem::AddComponentImmediate<Trigger3DComponent>(gEngine->GetWorld(), brick, gEngine->GetWorld(), reinterpret_cast<Physics3DShape*>(new Physics3DBoxShape(Vector(1, 1, 1))));
			DeferredTaskSystem::AddComponentImmediate<Rigidbody3DComponent>(gEngine->GetWorld(), brick, gEngine->GetWorld(), eRigidBody3DType::DYNAMIC, 10);
			DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, brick, "Models/Cube.fbx", eResourceSource::GAME);
	
			world->GetComponent<MeshRenderingComponent>(brick)->SetMaterial(0, PhongMaterial(Color(1, 0, 0.2), Color(1, 0, 0.2), Color(1, 0, 0.2), 8.0f));
			world->GetComponent<MeshRenderingComponent>(brick)->SetShadingModel(eShadingModel::LIT);
	
			TransformComponent* brickTransform = gEngine->GetWorld()->GetComponent<TransformComponent>(brick);
			brickTransform->SetLocalTranslation(Vector(-5 + i*2, 5 + j*2, 0));
			Rigidbody3DComponent* stoneRigidbody = gEngine->GetWorld()->GetComponent<Rigidbody3DComponent>(brick);
		}
	}
}

void BT::GameManagerSystem::SpawnBullet(Poly::World * world)
{
	GameManagerWorldComponent* gameManager = Poly::gEngine->GetWorld()->GetWorldComponent<GameManagerWorldComponent>();

	UniqueID bullet = DeferredTaskSystem::SpawnEntityImmediate(gEngine->GetWorld());
	gameManager->Bullets.PushBack(bullet);

	DeferredTaskSystem::AddComponentImmediate<StatusFlagsComponent>(gEngine->GetWorld(), bullet);
	DeferredTaskSystem::AddComponentImmediate<TransformComponent>(gEngine->GetWorld(), bullet);
	DeferredTaskSystem::AddComponentImmediate<Trigger3DComponent>(gEngine->GetWorld(), bullet, gEngine->GetWorld(), reinterpret_cast<Physics3DShape*>(new Physics3DSphereShape(1)));
	DeferredTaskSystem::AddComponentImmediate<Rigidbody3DComponent>(gEngine->GetWorld(), bullet, gEngine->GetWorld(), eRigidBody3DType::DYNAMIC, 1);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, bullet, "Models/Sphere.fbx", eResourceSource::GAME);

	world->GetComponent<MeshRenderingComponent>(bullet)->SetMaterial(0, PhongMaterial(Color(1, 0.2, 0), Color(1, 0.2, 0), Color(1, 0.2, 0), 8.0f));
	world->GetComponent<MeshRenderingComponent>(bullet)->SetShadingModel(eShadingModel::LIT);
}
