#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <ParticleEmitter.hpp>
#include <ResourceManager.hpp>
#include <DeferredTaskSystem.hpp>
#include <DebugDrawSystem.hpp>
#include <MeshRenderingComponent.hpp>
#include <CameraComponent.hpp>
#include <ParticleComponent.hpp>
#include <FreeFloatMovementComponent.hpp>
#include <LightSourceComponent.hpp>
#include <PostprocessSettingsComponent.hpp>
#include <ViewportWorldComponent.hpp>
#include <TimeWorldComponent.hpp>
#include <SkyboxWorldComponent.hpp>
#include <SpritesheetComponent.hpp>
#include <InputWorldComponent.hpp>

using namespace Poly;

void GameManagerSystem::CreateScene(World* world)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.2f, 0.5f, 1.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.05f);

	// Dir Light 0
	Quaternion DirLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	Entity* KeyDirLightEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, KeyDirLightEnt, Color(1.0f, 0.9f, 0.8f), 0.8f);
	EntityTransform& dirLightTrans = KeyDirLightEnt->GetTransform();
	dirLightTrans.SetLocalRotation(DirLightRot);
	GameMgrCmp->GameEntities.PushBack(KeyDirLightEnt);

	
	SpawnShip(world);

	for (int i = 0; i < 20; ++i)
	{
		Vector rnd = RandomVectorRange(0.0f, 1.0f) * 100.0f;
		SpawnBomb(world, Vector(rnd.X, 0.0f, rnd.Z));
	}
}

void GameManagerSystem::SpawnParticleExamples(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	// GameMgrCmp->particleDefault = SpawnEmitterDefault(world, Vector(0.0f, 0.0f, 0.0f));
	// GameMgrCmp->particleHeart = SpawnEmitterHeart(world, Vector(0.0f, 4.0f, 0.0f));
	// GameMgrCmp->particleHeartImpact0 = SpawnEmitterHeartImpact(world, Vector(0.0f, 4.0f, 0.0f));
	// GameMgrCmp->particleHeartImpact1 = SpawnEmitterHeartImpact(world, Vector(0.0f, 4.0f, 0.0f));
	// GameMgrCmp->particleHeartImpact2 = SpawnEmitterHeartImpact2(world, Vector(0.0f, 4.0f, 0.0f));
	// GameMgrCmp->particleWorldSpace = SpawnEmitterWorldSpace(world, Vector(0.0f, 0.0f, 0.0f));
	// GameMgrCmp->particleLocalSpace = SpawnEmitterLocalSpace(world, Vector(0.0f, 0.0f, 0.0f));

	// SpawnEmitterAmbientDust(world,	Vector( 0.0f, 0.0f,  0.0f));
	// SpawnEmitter1(world,			Vector(-5.0f, 0.0f, -4.0f));
	// SpawnEmitter2(world,			Vector( 0.0f, 0.0f, -4.0f));
	// SpawnEmitter3(world,			Vector(-5.0f, 0.0f, -4.0f));
}

void GameManagerSystem::SpawnShip(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ShipRootEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	Entity* ShipModelEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ShipModelEnt->SetParent(ShipRootEnt);
	EntityTransform& ShipModelTrans = ShipModelEnt->GetTransform();
	ShipModelTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
	ShipModelTrans.SetLocalScale(Vector(1.5f, 0.2f, 0.5f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, ShipModelEnt, "Models/Primitives/Cube.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ShipMesh = world->GetComponent<MeshRenderingComponent>(ShipModelEnt);
	ShipMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));

	Entity* ShipCollisionEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ShipCollisionEnt->SetParent(ShipRootEnt);
	EntityTransform& ShipCollisionTrans = ShipCollisionEnt->GetTransform();
	ShipCollisionTrans.SetLocalScale(1.0f);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, ShipCollisionEnt, "Models/Primitives/Sphere_Lowpoly.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ShipCollisionMesh = world->GetComponent<MeshRenderingComponent>(ShipCollisionEnt);
	ShipCollisionMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));
	
	GameMgrCmp->ShipRoot = ShipRootEnt;
	GameMgrCmp->ShipModel = ShipModelEnt;
	GameMgrCmp->ShipCollision = ShipCollisionEnt;
	GameMgrCmp->GameEntities.PushBack(ShipModelEnt);
	GameMgrCmp->GameEntities.PushBack(ShipRootEnt);
	GameMgrCmp->GameEntities.PushBack(ShipCollisionEnt);

	GameMgrCmp->ShipParticleSmoke = SpawnSmokeEmitterInWS(world, ShipRootEnt, Vector(-1.4f, 3.0f, 0.0f));
	GameMgrCmp->ShipParticleSmokeBurst = SpawnSmokeBurstEmitterInWS(world, ShipRootEnt, Vector(-1.4f, 3.0f, 0.0f));
	
	
	Entity* CameraRootHEnt = DeferredTaskSystem::SpawnEntityImmediate(world);

	Entity* CameraRootVEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	CameraRootVEnt->SetParent(CameraRootHEnt);

	Entity* CameraEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	CameraEnt->SetParent(CameraRootVEnt);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, CameraEnt, 45_deg, 0.1f, 3000.f);
	EntityTransform& CameraTrans = CameraEnt->GetTransform();
	CameraTrans.SetLocalTranslation(Vector(0.0f, 1.0f, 3.0f) * 5.0f);
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(CameraEnt));

	PostprocessSettingsComponent* postCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, CameraEnt);
	postCmp->UseBgShader = true;
	postCmp->UseFgShader = true;
	postCmp->Distortion = 0.5f;
	postCmp->ColorTempValue = 6500.0f;
	postCmp->Saturation = 1.0f;
	postCmp->Grain = 1.0f;
	postCmp->Stripes = 0.0f;
	postCmp->Vignette = 1.0f;

	GameMgrCmp->Camera = CameraEnt;
	GameMgrCmp->CameraRootH = CameraRootHEnt;
	GameMgrCmp->CameraRootV = CameraRootVEnt;
	
	GameMgrCmp->GameEntities.PushBack(CameraEnt);
	GameMgrCmp->GameEntities.PushBack(CameraRootHEnt);
	GameMgrCmp->GameEntities.PushBack(CameraRootVEnt);
}

void GameManagerSystem::SpawnBomb(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* BombEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& BombTrans = BombEnt->GetTransform();
	// ShipModelTrans.SetLocalRotation(Quaternion(Vector::UNIT_X, -90_deg) * Quaternion(Vector::UNIT_Z, -90_deg));
	BombTrans.SetLocalTranslation(pos);
	BombTrans.SetLocalScale(1.0f);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, BombEnt, "Models/Primitives/Sphere_LowPoly.fbx", eResourceSource::GAME);
	MeshRenderingComponent* BombMesh = world->GetComponent<MeshRenderingComponent>(BombEnt);
	BombMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));

	GameMgrCmp->GameEntities.PushBack(BombEnt);
	GameMgrCmp->BombEntities.PushBack(BombEnt);
}

#pragma region Particle Examples

ParticleComponent* GameManagerSystem::SpawnEmitterWorldSpace(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 50;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.05f;
	settings.BurstTimeMax = 0.20f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(0.5f, 2.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnSmokeEmitterInWS(World* world, Entity* parent, Vector offset)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ParticlesEnt->SetParent(parent);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(offset);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/puff2_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.1f;
	settings.BurstSizeMin = 3;
	settings.BurstSizeMin = 5;
	settings.Speed = 0.1f;
	settings.Color = Color(0.3f, 0.3f, 0.3f, 0.1f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 0.2f;
		Vector rndVel = RandomVectorRange(0.5f, 1.0f);
		p->Velocity = Vector(0.0f, 0.5f * rndVel.Y, 1.0f * rndVel.Z) * 0.1f;
		p->LifeTime = RandomRange(7.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp( 0.5f, 5.0f, t);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnSmokeBurstEmitterInWS(World* world, Entity* parent, Vector offset)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ParticlesEnt->SetParent(parent);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(offset);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/puff2_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.2f;
	settings.BurstSizeMin = 2;
	settings.BurstSizeMin = 8;
	settings.Speed = 0.1f;
	settings.Color = Color(0.1f, 0.1f, 0.1f, 0.2f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 0.2f;
		Vector rndVel = RandomVectorRange(0.5f, 1.0f);
		p->Velocity = Vector(0.0f, 0.5f * rndVel.Y, 1.0f * rndVel.Z) * 0.1f;
		p->LifeTime = RandomRange(3.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(0.2f, 2.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp(1.0f, 3.0f, t);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	particleCmp->GetEmitter()->SetBurstEnabled(false);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterLocalSpace(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Acceleration;
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEngineEmitterInWS(World* world, Entity* parent, Vector offset)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ParticlesEnt->SetParent(parent);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(offset);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/water2_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 500;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.1f;
	settings.BurstSizeMin = 3;
	settings.BurstSizeMin = 5;
	settings.Speed = 0.1f;
	settings.Color = Color(1.0f, 1.8f, 2.0f, 0.01f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 0.2f;
		Vector rndVel = RandomVectorRange(0.5f, 1.0f);
		p->Velocity = Vector(0.1f*rndVel.X, 0.2f * rndVel.Y, 0.1f * rndVel.Z) * 0.01f;
		p->LifeTime = RandomRange(0.1f, 0.5f);
		p->Scale = Vector::ONE * RandomRange(0.1f, 0.5f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp(0.5f, 2.0f, t);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEngineBurstEmitterInWS(World* world, Entity* parent, Vector offset)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ParticlesEnt->SetParent(parent);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(offset);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/water2_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 500;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.1f;
	settings.BurstSizeMin = 3;
	settings.BurstSizeMin = 5;
	settings.Speed = 0.1f;
	settings.Color = Color(1.0f, 1.8f, 2.0f, 0.1f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 0.2f;
		Vector rndVel = RandomVectorRange(0.5f, 1.0f);
		p->Velocity = Vector(0.1f*rndVel.X, 0.2f * rndVel.Y, 0.1f * rndVel.Z) * 0.01f;
		p->LifeTime = RandomRange(0.1f, 0.5f);
		p->Scale = Vector::ONE * RandomRange(0.1f, 0.5f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp(0.5f, 2.0f, t);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

#pragma endregion

void GameManagerSystem::Update(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();

	UpdateCamera(world);

	UpdateParticles(world);

	if (inputCmp->IsReleased(eKey::SPACE))
	{
		GameMgrCmp->SetIsPaused(!GameMgrCmp->GetIsPaused());
	}

	if (GameMgrCmp->GetIsPaused())
	{
		return;
	}

	if (GameMgrCmp->GetNeedRestart())
	{
		RestartGame(world);
	}

	UpdateShip(world);

	UpdateGameplay(world);
}

void GameManagerSystem::UpdateGameplay(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	PostprocessSettingsComponent* PostCmp = GameMgrCmp->Camera->GetComponent<PostprocessSettingsComponent>();

	for (SafePtr<Entity> Bomb : GameMgrCmp->BombEntities)
	{
		if (CollideWithBomb(GameMgrCmp->ShipCollision.Get(), Bomb.Get()))
		{
			GameMgrCmp->SetIsPaused(true);
			GameMgrCmp->SetNeedRestart(true);
			PostCmp->TimeOfDeath = time;
		}
	}
}

bool GameManagerSystem::CollideWithBomb(Entity* ShipCollision, Entity* Bomb)
{
	EntityTransform& ShipTrans = ShipCollision->GetTransform();
	Vector ShipPosition = ShipTrans.GetGlobalTranslation();
	float ShipRadius = 1.0f;
	
	EntityTransform& BombTrans = Bomb->GetTransform();
	Vector BombPosition = BombTrans.GetGlobalTranslation();
	float BombRadius = 1.0f;

	float Distance = (ShipPosition - BombPosition).Length();
	// gConsole.LogInfo("GameManagerSystem::CollideWithBomb Distance: {}", Distance );
	return Distance < (ShipRadius + BombRadius);
}

void GameManagerSystem::RestartGame(World* world)
{
	gConsole.LogInfo("GameManagerSystem::RestartGame");

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	EntityTransform& ShipModelTrans = GameMgrCmp->ShipRoot.Get()->GetTransform();
	ShipModelTrans.SetGlobalTranslation(Vector(0.0f, 0.0f, 0.0f));
	GameMgrCmp->SetShipAngleY(0.0f);
	GameMgrCmp->SetShipVelocity(1.0f);

	EntityTransform& CameraRootTrans = GameMgrCmp->CameraRootH->GetTransform();
	CameraRootTrans.SetGlobalTranslation(Vector(0.0f, 0.0f, 0.0f));
	GameMgrCmp->SetCamAngleV(0.0f);
	GameMgrCmp->SetCamAngleH(0.0f);
	GameMgrCmp->CameraRootV.Get()->GetTransform().SetLocalRotation(Quaternion::IDENTITY);
	GameMgrCmp->CameraRootH.Get()->GetTransform().SetLocalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg));

	PostprocessSettingsComponent* PostCmp = GameMgrCmp->Camera->GetComponent<PostprocessSettingsComponent>();
	PostCmp->ShipPos = GameMgrCmp->ShipRoot->GetTransform().GetGlobalTranslation();
	PostCmp->ShipAngleY = GameMgrCmp->GetShipAngleY();
	PostCmp->TimeOfDeath = -1.0f;

	for (SafePtr<Entity> Bomb : GameMgrCmp->BombEntities)
	{
		EntityTransform& BombTrans = Bomb.Get()->GetTransform();
		Vector rnd = RandomVectorRange(0.0f, 1.0f) * 100.0f;
		BombTrans.SetGlobalTranslation(Vector(rnd.X, 0.0f, rnd.Z));
	}

	GameMgrCmp->SetNeedRestart(false);
}

void GameManagerSystem::UpdateShip(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	float time = (float)(TimeSystem::GetTimerElapsedTime(world, Poly::eEngineTimer::GAMEPLAY));
	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	if (GameMgrCmp->ShipRoot != nullptr)
	{
		float InputAccel = 0.0f;
		float InputRotY = 0.0f;

		if (inputCmp->IsPressed(eKey::KEY_W))
			InputAccel += 1.0f;
		else if (inputCmp->IsPressed(eKey::KEY_S))
			InputAccel -= 1.0f;

		if (inputCmp->IsPressed(eKey::KEY_A))
			InputRotY += 1.0f;
		else if (inputCmp->IsPressed(eKey::KEY_D))
			InputRotY -= 1.0f;


		bool IsInputActive = Abs(InputAccel) > 0.01f; // || Abs(InputRotY) > 0.01f;
		GameMgrCmp->ShipParticleSmokeBurst->GetEmitter()->SetBurstEnabled(IsInputActive);
		// GameMgrCmp->ShipParticleEngineBurst->GetEmitter()->SetBurstEnabled(IsInputActive);
		
		// Vector Velocity = Vector::UNIT_X * (2.0f * deltaTime * InputAccel + 2.0f + 0.1f * Abs(Sin(0.1_rad * time)));
		float Velocity = 0.2f * deltaTime * InputAccel + GameMgrCmp->GetShipVelocity();
		// float sign = Velocity > 0.0f ? 1.0 : (Velocity < 0.0f ? -1.0 : 0.0f);
		// Velocity -= sign * 0.01f + 0.01f * Abs(Sin(0.1_rad * time));
		Velocity *= 0.99f;
		Velocity = Clamp(Velocity, -0.5f, 0.5f);
		GameMgrCmp->SetShipVelocity(Velocity);
		Vector VelocityVector = Vector::UNIT_X * Velocity;
		VelocityVector -= Vector(0.1f, 0.0f, -0.5f) * 0.02f;
		EntityTransform& ShipRootTrans = GameMgrCmp->ShipRoot->GetTransform();
		ShipRootTrans.SetGlobalTranslation(ShipRootTrans.GetGlobalTranslation() + ShipRootTrans.GetGlobalRotation() * VelocityVector);
		
		float RotYValue = 1.0f * deltaTime * InputRotY + GameMgrCmp->GetShipAngleY(); // rad;
		// RotYValue += 0.02f * deltaTime;
		RotYValue -= 0.01f * Sin(0.01_rad * time); // rad
		GameMgrCmp->SetShipAngleY(RotYValue);
		Angle RotY = 1.0_rad * RotYValue;
		ShipRootTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, RotY));

		PostprocessSettingsComponent* PostCmp = GameMgrCmp->Camera->GetComponent<PostprocessSettingsComponent>();
		PostCmp->ShipPos = GameMgrCmp->ShipRoot->GetTransform().GetGlobalTranslation();
		PostCmp->ShipAngleY = -RotYValue;
		// gConsole.LogDebug("GameManagerSystem::UpdateCamera: ShipRot: {}", RotYValue );
	}
}

void GameManagerSystem::UpdateCamera(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();

	if (GameMgrCmp->Camera != nullptr
		&&	GameMgrCmp->CameraRootH != nullptr
		&&	GameMgrCmp->CameraRootV != nullptr)
	{
		EntityTransform& CameraTrans = GameMgrCmp->Camera->GetTransform();
		EntityTransform& CameraRootHTrans = GameMgrCmp->CameraRootH->GetTransform();
		EntityTransform& CameraRootVTrans = GameMgrCmp->CameraRootV->GetTransform();

		Vector2i mouseDelta = inputCmp->GetMousePosDelta();

		float AngularVel = GameMgrCmp->GetAngularVelocity();
		float AngleH = GameMgrCmp->GetCamAngleH();
		float AngleV = GameMgrCmp->GetCamAngleV();

		AngleH += -mouseDelta.X * AngularVel;
		AngleV += -mouseDelta.Y * AngularVel;
		AngleV = Clamp(AngleV, -38.0f, 20.0f);

		Quaternion rotH = Quaternion(Vector::UNIT_Y, 1.0_deg * AngleH);
		Quaternion rotV = Quaternion(Vector::UNIT_X, 1.0_deg * AngleV);

		CameraRootVTrans.SetLocalRotation(rotV);
		CameraRootHTrans.SetLocalRotation(rotH);

		GameMgrCmp->SetCamAngleH(AngleH);
		GameMgrCmp->SetCamAngleV(AngleV);

		EntityTransform& ShipRootTrans = GameMgrCmp->ShipRoot->GetTransform();
		EntityTransform& CameraRootTrans = GameMgrCmp->CameraRootH->GetTransform();

		// float posDiff = (CameraRootTrans.GetGlobalTranslation() - ShipRootTrans.GetGlobalTranslation()).LengthSquared();
		// if (posDiff > 0.1f)
		// {
		CameraRootTrans.SetGlobalTranslation(Lerp(
			CameraRootTrans.GetGlobalTranslation(),
			ShipRootTrans.GetGlobalTranslation(),
			0.02f
		));
		// }
	}
}

void GameManagerSystem::UpdateParticles(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Vector translation = Vector::UNIT_Y * (0.2f*Abs(Sin(1.0_rad*3.1415f *time + 0.5_rad)));
	Vector scale = Vector::ONE * (1.0f + 0.1f*(0.5f + 0.5f*Sin(2.0_rad*3.1415f *time + 0.5_rad)));
	Quaternion rotation = Quaternion(Vector::UNIT_Y, 0.5_rad * time);

	// if (GameMgrCmp->particleHeart)
	// {
	// 	GameMgrCmp->particleHeart->GetTransform().SetGlobalTranslation(translation);
	// 	GameMgrCmp->particleHeart->GetTransform().SetGlobalScale(scale);
	// 	GameMgrCmp->particleHeart->GetTransform().SetGlobalRotation(rotation);
	// }
	// 
	// if (GameMgrCmp->particleHeartImpact0)
	// {
	// 	GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalTranslation(translation);
	// 	GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalScale(scale);
	// 	GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalRotation(rotation);
	// }
	// 
	// if (GameMgrCmp->particleHeartImpact1)
	// {
	// 	GameMgrCmp->particleHeartImpact1->GetTransform().SetGlobalRotation(rotation);
	// }
	// 
	// if (GameMgrCmp->particleHeartImpact2)
	// {
	// 	GameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * 0.2f);
	// 	GameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalRotation(rotation);
	// }
	// 
	// if (GameMgrCmp->particleLocalSpace)
	// {
	// 	GameMgrCmp->particleLocalSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time), 0.0f, Sin(100.0_deg * time)) * 6.0f);
	// }

	if (GameMgrCmp->particleWorldSpace)
	{
		GameMgrCmp->particleWorldSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time + 180.0_deg), 0.0f, Sin(100.0_deg * time + 180.0_deg)) * 6.0f);
	}
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Cleanup");
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	for (SafePtr<Entity> e : GameMgrCmp->GameEntities)
	{
		DeferredTaskSystem::DestroyEntityImmediate(world, e.Get());
	}
}
