#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <ParticleEmitter.hpp>
#include <ResourceManager.hpp>
#include <DeferredTaskSystem.hpp>
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

	// EnumArray<String, eCubemapSide> stormydays{
	// 	{eCubemapSide::RIGHT, "Cubemaps/stormydays/stormydays_rt.jpg"},
	// 	{eCubemapSide::LEFT , "Cubemaps/stormydays/stormydays_lt.jpg"},
	// 	{eCubemapSide::TOP  , "Cubemaps/stormydays/stormydays_up.jpg"},
	// 	{eCubemapSide::DOWN , "Cubemaps/stormydays/stormydays_dn.jpg"},
	// 	{eCubemapSide::BACK , "Cubemaps/stormydays/stormydays_bk.jpg"},
	// 	{eCubemapSide::FRONT, "Cubemaps/stormydays/stormydays_ft.jpg"}
	// };
	// DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, stormydays);

	/*
	Entity* DirLightEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, DirLightEnt, Color(0.8f, 0.8f, 1.0f), 0.3);
	DirLightEnt->GetTransform().SetLocalRotation(Quaternion(Vector::UNIT_X, -30_deg));

	Entity* DirLightEnt1 = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, DirLightEnt1, Color(1.0f, 0.8f, 0.8f), 0.1);
	DirLightEnt1->GetTransform().SetLocalRotation(Quaternion(Vector::UNIT_X, -140_deg));
	*/

	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.0f, 0.0f, 0.1f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(1.0f);

	
	SpawnShip(world);
	
	// SpawnSponzaScene(world);

	// SpawnSpritesheerExamples(world);
	// SpawnParticleExamples(world);

	// CreateShaderball(world, GameMgrCmp);
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

void GameManagerSystem::SpawnSpritesheerExamples(World* world)
{
	Vector spritesheetsPosition = Vector(-15.0f, 0.0f, 0.0f);
	SpawnSpritesheet11(world, spritesheetsPosition + Vector(-5.0f, 4.0f, 0.0f));
	SpawnSpritesheet22(world, spritesheetsPosition + Vector(0.0f, 4.0f, 0.0f));
	SpawnSpritesheet44(world, spritesheetsPosition + Vector(5.0f, 4.0f, 0.0f));
	SpawnSpritesheet42(world, spritesheetsPosition + Vector(-5.0f, 8.0f, 0.0f));
	SpawnSpritesheet41(world, spritesheetsPosition + Vector(0.0f, 8.0f, 0.0f));
	SpawnSpritesheet44Random(world, spritesheetsPosition + Vector(5.0f, 8.0f, 0.0f));
	SpawnSpritesheetGandalf(world, spritesheetsPosition + Vector(0.0f, 12.0f, 0.0f));
}

void GameManagerSystem::SpawnShip(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ShipRootEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	Entity* ShipModelEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	ShipModelEnt->SetParent(ShipRootEnt);
	EntityTransform& ShipModelTrans = ShipModelEnt->GetTransform();
	// ShipModelTrans.SetLocalRotation(Quaternion(Vector::UNIT_X, -90_deg) * Quaternion(Vector::UNIT_Z, -90_deg));
	// ShipTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
	ShipModelTrans.SetLocalScale(Vector(2.0f, 0.5f, 1.0f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, ShipModelEnt, "Models/Primitives/Cube.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ShipMesh = world->GetComponent<MeshRenderingComponent>(ShipModelEnt);
	ShipMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));
	
	// Entity* ShipCanonEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	// ShipCanonEnt->SetParent(ShipRootEnt);
	// EntityTransform& ShipCanonTrans = ShipCanonEnt->GetTransform();
	// ShipModelTrans.SetLocalRotation(Quaternion(Vector::UNIT_X, -90_deg) * Quaternion(Vector::UNIT_Z, -90_deg));
	// ShipCanonTrans.SetLocalTranslation(Vector(0.0f, 1.0f, 0.0f));
	// ShipCanonTrans.SetLocalScale(0.5f);
	// DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, ShipCanonEnt, "Models/Primitives/Canon.fbx", eResourceSource::GAME);
	// MeshRenderingComponent* ShipCanonMesh = world->GetComponent<MeshRenderingComponent>(ShipCanonEnt);
	// ShipCanonMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));

	GameMgrCmp->ShipRoot = ShipRootEnt;
	GameMgrCmp->ShipModel = ShipModelEnt;
	// GameMgrCmp->ShipCanon = ShipCanonEnt;
	GameMgrCmp->GameEntities.PushBack(ShipModelEnt);
	GameMgrCmp->GameEntities.PushBack(ShipRootEnt);

	GameMgrCmp->ShipParticleSmoke = SpawnSmokeEmitterInWS(world, ShipRootEnt, Vector(1.2f, 3.0f, 0.0f));
	GameMgrCmp->ShipParticleSmokeBurst = SpawnSmokeBurstEmitterInWS(world, ShipRootEnt, Vector(1.2f, 3.0f, 0.0f));
	GameMgrCmp->ShipParticleEngine = SpawnEngineEmitterInWS(world, ShipRootEnt, Vector(4.0f, 0.0f, 0.0f));
	GameMgrCmp->ShipParticleEngineBurst = SpawnEngineBurstEmitterInWS(world, ShipRootEnt, Vector(4.0f, 0.0f, 0.0f));
	
	
	Entity* CameraRootHEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	// CameraRootHEnt->SetParent(ShipRootEnt);

	Entity* CameraRootVEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	CameraRootVEnt->SetParent(CameraRootHEnt);

	Entity* CameraEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	CameraEnt->SetParent(CameraRootVEnt);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, CameraEnt, 90_deg, 0.1f, 3000.f);
	EntityTransform& CameraTrans = CameraEnt->GetTransform();
	CameraTrans.SetLocalTranslation(Vector(0.0f, 1.0f, 2.0f) * 3.0f);
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


#pragma region Particle Examples

ParticleComponent* GameManagerSystem::SpawnEmitterDefault(World* world, Vector pos)
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
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeart(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	// ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";
	spriteSettings.Speed = 0.0f;

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Color = Color(1.2f, 0.8f, 0.8f, 0.5f);
	settings.BurstTimeMin = 0.01f;
	settings.BurstTimeMax = 0.05f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 30;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rnd = RandomVectorRange(-1.0f, 1.0f);
		rnd.Normalize();
		rnd.Z = rnd.Z * 0.5f;
		rnd.Z = rnd.Z * (1.0f - rnd.Y * 1.0f);
		rnd.Y = 0.9f*rnd.Y - Abs(rnd.X) * sqrt((20.0f - Abs(rnd.X))/15.0f);
		rnd.Y *= -0.8f;
		p->Position += rnd;
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(3.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(0.02f, 0.2f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeartImpact(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	// ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Color = Color(1.5f, 1.0f, 1.0f, 0.95f);
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.5f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 30;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rndPos = RandomVectorRange(-1.0f, 1.0f);
		rndPos.Normalize();
		rndPos.Z = rndPos.Z * 0.5f;
		rndPos.Z = rndPos.Z * (1.0f - rndPos.Y * 1.0f);
		rndPos.Y = 0.9f*rndPos.Y - Abs(rndPos.X) * sqrt((20.0f - Abs(rndPos.X)) / 15.0f);
		rndPos.Y *= -0.8f;
		p->Position += rndPos;
		Vector rndVel = RandomVectorRange(-1.0f, 1.0f);
		rndVel.Normalize();
		p->Velocity = rndVel * 0.01f;
		p->Acceleration = rndVel * 0.0001f;
		p->LifeTime = RandomRange(8.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.01f, 0.02f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		p->Scale = Vector::ONE * Lerp(0.04f, 0.01f, pow(p->Age / p->LifeTime, 16.0f));
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeartImpact2(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	// ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/strokes_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 1000;
	settings.Color = Color(2.0f, 0.5f, 0.5f, 0.2f);
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 1.0f;
	settings.BurstSizeMin = 200;
	settings.BurstSizeMax = 200;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		Vector rndPos = RandomVectorRange(-1.0f, 1.0f);
		rndPos.Normalize();
		rndPos.Z = rndPos.Z * 0.5f;
		rndPos.Z = rndPos.Z * (1.0f - rndPos.Y * 1.0f);
		rndPos.Y = 0.9f*rndPos.Y - Abs(rndPos.X) * sqrt((20.0f - Abs(rndPos.X)) / 15.0f);
		rndPos.Y *= -0.8f;
		rndPos.Y += 0.02f;
		// rndPos *= 1.2f;
		Vector rndVel = rndPos;
		Vector rndAccel = RandomVectorRange(0.5f, 1.0f);
		p->Position += rndPos;
		p->Velocity = rndVel * 0.01f;
		p->Acceleration = rndVel * 0.001f + Vector(-1.5f * rndAccel.X, 1.0f * rndAccel.Y, 0.5f * rndAccel.Z) * 0.001f;
		p->LifeTime = RandomRange(0.2f, 0.75f);
		p->Scale = Vector::ONE * RandomRange(0.04f, 0.05f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		p->Scale = Vector::ONE * Lerp(0.4f, 0.001f, pow(p->Age / p->LifeTime, 8.0f));
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

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
		p->LifeTime = RandomRange(7.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.2f, 2.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		// p->Position += p->Velocity;
		float t = p->Age / p->LifeTime;
		p->Scale = Vector::ONE * Lerp(1.0f, 10.0f, t);
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


#pragma region Spritesheet examples

void GameManagerSystem::SpawnSpritesheet11(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(1.0f, 1.0f);
	settings.SpritePath = "Textures/test_1_1.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet22(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(2.0f, 2.0f);
	settings.SpritePath = "Textures/test_2_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.Color = Color(0.0f, 1.0f, 0.0f, 0.5f);
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet42(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 2.0f);
	settings.SpritePath = "Textures/test_4_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet41(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 1.0f);
	settings.SpritePath = "Textures/test_4_1.png";
	settings.Color = Color::RED;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44Random(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.IsRandom = true;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheetGandalf(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = SpriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/gandalf_anim.png";
	settings.Speed = 0.2f;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

#pragma endregion


void GameManagerSystem::SpawnShaderball(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Shaderball = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& shaderballTrans = Shaderball->GetTransform();
	shaderballTrans.SetLocalTranslation(Vector(0.0f, 5.0f, 0.0f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Shaderball, "Models/shaderball/PolyEngine_shaderball.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ballMesh = world->GetComponent<MeshRenderingComponent>(Shaderball);
	ballMesh->SetMaterial(0, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), Color(1.0f, 1.0f, 0.5f), 8.0f));
	ballMesh->SetMaterial(1, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(0.4f, 0.4f, 0.4f), Color(1.0f, 1.0f, 0.5f), 16.0f));
	shaderballTrans.SetLocalScale(0.1f);
	GameMgrCmp->GameEntities.PushBack(Shaderball);
}

void GameManagerSystem::SpawnSponzaScene(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.2f, 0.5f, 1.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.05f);

	// Dir Light 0
	Quaternion DirLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	Entity* KeyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, KeyDirLight, Color(1.0f, 0.9f, 0.8f), 0.8f);
	EntityTransform& dirLightTrans = KeyDirLight->GetTransform();
	dirLightTrans.SetLocalRotation(DirLightRot);
	GameMgrCmp->KeyDirLight = KeyDirLight;

	// Point Lights
	// CreatePointLight(world, 100.0f);
	// AddPointLights(world, 7);
	// CreateSpotLight(world, 200.0f);

	Entity* Ground = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Ground, "Models/Primitives/Plane.fbx", eResourceSource::GAME);
	Ground->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_X, -90_deg)/* * Quaternion(Vector::UNIT_Z, -90_deg)*/);
	Ground->GetTransform().SetGlobalScale(Vector(100.0f, 100.0f, 1.0f));
	MeshRenderingComponent* GroundMesh = world->GetComponent<MeshRenderingComponent>(Ground);
	GroundMesh->SetMaterial(0, PhongMaterial(Color(0.2f, 0.2f, 0.5f), Color(0.2f, 0.2f, 0.5f), Color(0.2f, 0.2f, 0.5f), 8.0f));
	GameMgrCmp->GameEntities.PushBack(Ground);
}

void GameManagerSystem::Update(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (int i = 0; i < GameMgrCmp->PointLights.GetSize(); ++i)
	{
		PointLightComponent* PointLightCmp = GameMgrCmp->PointLights[i];
		EntityTransform& TransCmp = PointLightCmp->GetTransform();
		Vector Position = GameMgrCmp->PointLightPositions[i];
		Vector Bounce = Vector(0.0f, 100.0f*Abs(Sin(1.0_rad * (time + 0.1f*(Position.X + Position.Y)))), 0.0f);
		TransCmp.SetLocalTranslation(Position + Bounce);
	}

	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	if (inputCmp->IsReleased(eKey::KEY_G))
	{
		GameMgrCmp->IsDrawingDebugMeshes = !GameMgrCmp->IsDrawingDebugMeshes;

		gConsole.LogInfo("GameManagerSystem::Update IsDrawingDebugMeshes: {}", GameMgrCmp->IsDrawingDebugMeshes);

		for (int i = 0; i < GameMgrCmp->DebugMeshes.GetSize(); ++i)
		{
			GameMgrCmp->DebugMeshes[i]->SetShadingModel(GameMgrCmp->IsDrawingDebugMeshes ? eShadingModel::UNLIT : eShadingModel::NONE);
		}
	}

	UpdateCamera(world);

	UpdateParticles(world);
}

void GameManagerSystem::UpdateCamera(World* world)
{
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	float time = (float)(TimeSystem::GetTimerElapsedTime(world, Poly::eEngineTimer::GAMEPLAY));
	InputWorldComponent* inputCmp = world->GetWorldComponent<InputWorldComponent>();
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	/*
	int wheelDelta = inputCmp->GetWheelPosDelta().Y;
	float speed = GameMgrCmp->GetMovementSpeed();
	speed = Clamp(speed + wheelDelta, 0.001f, 10000.0f);
	GameMgrCmp->SetMovementSpeed(speed);
	*/

	
	if (GameMgrCmp->ShipRoot != nullptr)
	{
		Vector AccelImpulse;
		if (inputCmp->IsPressed(eKey::KEY_W))
			AccelImpulse -= Vector::UNIT_Z;
		else if (inputCmp->IsPressed(eKey::KEY_S))
			AccelImpulse += Vector::UNIT_Z;

		if (inputCmp->IsPressed(eKey::KEY_A))
			AccelImpulse -= Vector::UNIT_X;
		else if (inputCmp->IsPressed(eKey::KEY_D))
			AccelImpulse += Vector::UNIT_X;

		/*
		if (inputCmp->IsPressed(eKey::KEY_Q))
			move -= Vector::UNIT_Y;
		else if (inputCmp->IsPressed(eKey::KEY_E))
			move += Vector::UNIT_Y;

		if (move.LengthSquared() > 0)
			move.Normalize();
		*/
		bool IsInputActive = AccelImpulse.LengthSquared() > 0.01f;
		GameMgrCmp->ShipParticleSmokeBurst->GetEmitter()->SetBurstEnabled(IsInputActive);
		GameMgrCmp->ShipParticleEngineBurst->GetEmitter()->SetBurstEnabled(IsInputActive);

		EntityTransform& ShipRootTrans = GameMgrCmp->ShipRoot->GetTransform();
		
		Vector Velocity = GameMgrCmp->GetVelocity();
		Vector Accel = GameMgrCmp->GetAccel();
		Accel *= 0.99f; // drag
		Accel += AccelImpulse * 0.1f;
		Velocity += Accel - Velocity * GameMgrCmp->GetFriction();

		// float VelocityValue = Velocity.Length();
		// VelocityValue = Clamp(VelocityValue, 0.0f, 10.0f); // Clamp max velocity
		// Velocity = Velocity.Normalize() * VelocityValue;

		GameMgrCmp->SetVelocity(Velocity);
		GameMgrCmp->SetAccel(Accel);

		if (Accel.LengthSquared() > 0.01f)
		{
			Vector ShipForward = MovementSystem::GetGlobalForward(ShipRootTrans);
			float RotAngle = Clamp(ShipForward.Dot(Accel.Normalize()), -0.5f, 0.5f);
			ShipRootTrans.SetGlobalRotation(Quaternion::Slerp(
				ShipRootTrans.GetGlobalRotation(),
				ShipRootTrans.GetGlobalRotation() * Quaternion(Vector::UNIT_Y, 1.0_rad * RotAngle ),
				15.0f * deltaTime
			));
		}
		

		ShipRootTrans.SetGlobalTranslation(ShipRootTrans.GetGlobalTranslation() + ShipRootTrans.GetGlobalRotation() * Velocity * deltaTime);

		EntityTransform& CameraRootTrans = GameMgrCmp->CameraRootH->GetTransform();
		CameraRootTrans.SetGlobalTranslation(Lerp(
			CameraRootTrans.GetGlobalTranslation(),
			ShipRootTrans.GetGlobalTranslation(),
			5.0f * deltaTime
		));

		// gConsole.LogInfo("GameManagerSystem::UpdateCamera ShipRoot: {}, CameraRoot: {}",
		// 	ShipRootTrans.GetGlobalTranslation(),
		// 	CameraRootTrans.GetGlobalTranslation()
		// );
	}


	/*
	if (inputCmp->IsPressed(eMouseButton::LEFT))
	{
		Vector2i delta = inputCmp->GetMousePosDelta();

		Quaternion rot = Quaternion(Vector::UNIT_Y, Angle::FromRadians(-delta.X * freeFloatMovementCmp->GetAngularVelocity()));
		rot *= trans.GetLocalRotation();
		rot *= Quaternion(Vector::UNIT_X, Angle::FromRadians(-delta.Y * freeFloatMovementCmp->GetAngularVelocity()));

		if (rot != Quaternion()) {
			rot.Normalize();
			trans.SetLocalRotation(rot);
		}
	}
	*/

	if (	GameMgrCmp->Camera != nullptr
		&&	GameMgrCmp->CameraRootH != nullptr
		&&	GameMgrCmp->CameraRootV != nullptr)
	{
		EntityTransform& CameraTrans = GameMgrCmp->Camera->GetTransform();
		EntityTransform& CameraRootHTrans = GameMgrCmp->CameraRootH->GetTransform();
		EntityTransform& CameraRootVTrans = GameMgrCmp->CameraRootV->GetTransform();

		Vector2i mouseDelta = inputCmp->GetMousePosDelta();

		float AngularVel = GameMgrCmp->GetAngularVelocity();
		float AngleH = GameMgrCmp->GetAngleH();
		float AngleV = GameMgrCmp->GetAngleV();
		AngleH += -mouseDelta.X * AngularVel;
		AngleV += -mouseDelta.Y * AngularVel;

		AngleV = Clamp(AngleV, -38.0f, 20.0f);

		Quaternion rotH = Quaternion(Vector::UNIT_Y, 1.0_deg * AngleH);
		Quaternion rotV = Quaternion(Vector::UNIT_X, 1.0_deg * AngleV);

		CameraRootVTrans.SetLocalRotation(rotV);
		CameraRootHTrans.SetLocalRotation(rotH);
		// CameraTrans.SetGlobalRotation(Quaternion::LookAt(CameraTrans.GetGlobalTranslation(), Vector(0.0f, 5.0f*Sin(100_deg * time), 0.0f)));

		// gConsole.LogDebug("GameManagerSystem::UpdateCamera: preRot: ({}, {}), delta({}, {}), postRot: ({}, {})",
		// 	GameMgrCmp->GetAngleH(), GameMgrCmp->GetAngleV(),
		// 	mouseDelta.X, mouseDelta.Y,
		// 	AngleH, AngleV
		// );
		GameMgrCmp->SetAngleH(AngleH);
		GameMgrCmp->SetAngleV(AngleV);

		
		/*
		Vector CameraForward = MovementSystem::GetGlobalForward(CameraTrans);
		// CameraForward.Y = 0.0f;
		// CameraForward.Normalize();
		
		if (CameraForward.LengthSquared() > 0.01)
		{
			Vector CanonTarget = CameraForward;
			CanonTarget.X = -CanonTarget.X;
			CanonTarget.Z = -CanonTarget.Z;
			EntityTransform& CanonTrans = GameMgrCmp->ShipCanon->GetTransform();
			CanonTrans.SetGlobalRotation(Quaternion::LookAt(
				CanonTrans.GetGlobalTranslation(),
				CanonTrans.GetGlobalTranslation() + CanonTarget
			));
		}
		*/
	}

	PostprocessSettingsComponent* PostCmp = GameMgrCmp->Camera->GetComponent<PostprocessSettingsComponent>();
	PostCmp->ShipPos = GameMgrCmp->ShipRoot->GetTransform().GetGlobalTranslation();
	EulerAngles Angles = GameMgrCmp->ShipRoot->GetTransform().GetGlobalRotation().ToEulerAngles();
	Angles.X = 0.0_rad;
	Angles.Z = 0.0_rad;
	PostCmp->ShipPos.W = Angles.Y.AsRadians();
	// PostCmp->ShipFwd = MovementSystem::GetGlobalForward(GameMgrCmp->ShipRoot->GetTransform());
	gConsole.LogDebug("GameManagerSystem::UpdateCamera: ShipRot: {}",
		GameMgrCmp->ShipRoot->GetTransform().GetGlobalRotation().ToEulerAngles()
	);
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
		DeferredTaskSystem::DestroyEntity(world, e.Get());
	}
}

void GameManagerSystem::AddPointLights(World* world, int Quota)
{
	for (int i = 0; i < Quota; ++i)
	{
		CreatePointLight(world, 200.0f);
	}
}

void GameManagerSystem::CreatePointLight(World* world, float Range)
{
	Vector PointLightPos = Vector(RandomRange(-1.0f, 1.0f), RandomRange(0.0f, 0.2f), RandomRange(-0.5f, 0.5f)) * 1000.0f;
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0, 0.5f), RandomRange(0.0f, 0.2f));

	Entity* PointLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, PointLight, LightColor, 1.0f, Range);
	EntityTransform& PointLightTrans = PointLight->GetTransform();
	PointLightTrans.SetLocalTranslation(PointLightPos);

	Entity* PointLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, PointLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* PointLightMesh = world->GetComponent<MeshRenderingComponent>(PointLightDebugSource);
	PointLightMesh->SetShadingModel(eShadingModel::UNLIT);
	PointLightMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& PointLightDebugSourceTrans = PointLightDebugSource->GetTransform();
	PointLightDebugSource->SetParent(PointLight);
	PointLightDebugSourceTrans.SetLocalScale(10.0f);
	PointLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));

	Entity* PointLightDebugRange = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, PointLightDebugRange, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* PointLightRangeMesh = world->GetComponent<MeshRenderingComponent>(PointLightDebugRange);
	PointLightRangeMesh->SetShadingModel(eShadingModel::UNLIT);
	PointLightRangeMesh->SetIsWireframe(true);
	PointLightRangeMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& PointLightRangeTrans = PointLightDebugRange->GetTransform();
	PointLightDebugRange->SetParent(PointLight);
	PointLightRangeTrans.SetLocalScale(Range);
	PointLightRangeTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));

	PointLightComponent* PointLightCmp = world->GetComponent<PointLightComponent>(PointLight);
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	GameMgrCmp->PointLights.PushBack(PointLightCmp);
	GameMgrCmp->PointLightPositions.PushBack(PointLightPos);
	GameMgrCmp->DebugMeshes.PushBack(PointLightRangeMesh);
}

void GameManagerSystem::CreateSpotLight(World* world, float Range)
{
	Vector SpotLightPos = Vector(50.0f, 50.0f, 0.0f);
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0, 0.5f), RandomRange(0.0f, 0.2f));
	Quaternion SpotLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	// float PointLightRange = 100.0f;
	Entity* SpotLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<SpotLightComponent>(world, SpotLight, LightColor, 1.0f, Range, 5.0f, 17.0f);
	EntityTransform& SpotLightTrans = SpotLight->GetTransform();
	SpotLightTrans.SetLocalTranslation(SpotLightPos);
	SpotLightTrans.SetLocalRotation(SpotLightRot);

	Entity* SpotLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, SpotLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* SpotLightMesh = world->GetComponent<MeshRenderingComponent>(SpotLightDebugSource);
	SpotLightMesh->SetShadingModel(eShadingModel::UNLIT);
	SpotLightMesh->SetMaterial(0, PhongMaterial(LightColor, LightColor, LightColor, 8.0f));
	EntityTransform& SpotLightDebugSourceTrans = SpotLightDebugSource->GetTransform();
	SpotLightDebugSource->SetParent(SpotLight);
	SpotLightDebugSourceTrans.SetLocalScale(2.0f);
	SpotLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
}
