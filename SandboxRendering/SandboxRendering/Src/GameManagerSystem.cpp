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

	srand(42);

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 90_deg, 1.0f, 3000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 1.0f, 0.003f);
	DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, Camera);
	PostprocessSettingsComponent* postCmp = world->GetComponent<PostprocessSettingsComponent>(Camera);
	postCmp->UseBgShader = false;
	postCmp->UseFgShader = false;
	postCmp->Distortion = 0.5f;
	postCmp->ColorTempValue = 6500.0f;
	postCmp->Saturation = 1.0f;
	postCmp->Grain = 1.0f;
	postCmp->Stripes = 0.0f;
	postCmp->Vignette = 1.0f;

	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetLocalTranslation(Vector(0.0f, 4.0f, 5.0f));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	EnumArray<String, eCubemapSide> miramar{
		{eCubemapSide::RIGHT, "Cubemaps/miramar/miramar_rt.jpg"},
		{eCubemapSide::LEFT , "Cubemaps/miramar/miramar_lt.jpg"},
		{eCubemapSide::TOP  , "Cubemaps/miramar/miramar_up.jpg"},
		{eCubemapSide::DOWN , "Cubemaps/miramar/miramar_dn.jpg"},
		{eCubemapSide::BACK , "Cubemaps/miramar/miramar_bk.jpg"},
		{eCubemapSide::FRONT, "Cubemaps/miramar/miramar_ft.jpg"}
	};

	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, miramar);

	world->GetWorldComponent<AmbientLightWorldComponent>()->SetColor(Color(0.0f, 0.0f, 0.0f));
	world->GetWorldComponent<AmbientLightWorldComponent>()->SetIntensity(0.0f);

	// GameMgrCmp->particle0 = SpawnEmitter0(world);
	// GameMgrCmp->particle1 = SpawnEmitter1(world);
	// GameMgrCmp->particle2 = SpawnEmitter2(world);
	// GameMgrCmp->particle3 = SpawnEmitter3(world);
	// GameMgrCmp->particle4 = SpawnEmitter4(world);

	// CreateShaderball(world, GameMgrCmp);

	SpawnSpritesheet11(world);
	SpawnSpritesheet22(world);
	SpawnSpritesheet44(world);
	SpawnSpritesheetGandalf(world);

	SpawnSponzaScene(world);
}

void GameManagerSystem::SpawnSpritesheet11(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = SpriteSheetEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(-5.0f, 4.0f, 0.0f));
	ParticlesEnt1Trans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetComponent::Settings settings;
	settings.SubImages = Vector2f(1.0f, 1.0f);
	settings.SpritePath = "Textures/test_1_1.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet22(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = SpriteSheetEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(0.0f, 4.0f, 0.0f));
	ParticlesEnt1Trans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetComponent::Settings settings;
	settings.SubImages = Vector2f(2.0f, 2.0f);
	settings.SpritePath = "Textures/test_2_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = SpriteSheetEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(5.0f, 4.0f, 0.0f));
	ParticlesEnt1Trans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetComponent::Settings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.Source = eResourceSource::GAME;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheetGandalf(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* SpriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = SpriteSheetEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(0.0f, 8.0f, 0.0f));
	ParticlesEnt1Trans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetComponent::Settings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/gandalf_anim.png";
	settings.Speed = 0.2f;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, SpriteSheetEnt, settings);
	SpritesheetComponent* SpritesheetComp = world->GetComponent<SpritesheetComponent>(SpriteSheetEnt);
	GameMgrCmp->GameEntities.PushBack(SpriteSheetEnt);
}


ParticleComponent* GameManagerSystem::SpawnEmitter0(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt0Trans = ParticlesEnt->GetTransform();
	ParticlesEnt0Trans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));

	ParticleEmitter::Settings settings;
	settings.InitialSize = 1000;
	settings.BurstSizeMin = 1;
	settings.BurstSizeMax = 10;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = Vector(0.0f, 0.0f, 0.0f) + (GameManagerSystem::RandomVector(-1.0f, 1.0f) * 20.0f);
		p->Acceleration = GameManagerSystem::RandomVector(-1.0f, 1.0f) * 0.005f;
		p->LifeTime = Random(100.0f, 200.0f);
		p->Scale = Vector(1.0f, 1.0f, 1.0f) * (0.05f + Random(0.0f, 0.05f));
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Position += p->Acceleration;
	};

	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	ParticleComponent* ParticleComp = world->GetComponent<ParticleComponent>(ParticlesEnt);
	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);
	return ParticleComp;
}

ParticleComponent* GameManagerSystem::SpawnEmitter1(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = ParticlesEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(-20.0f, 0.0f, 0.0f));
	ParticleEmitter::Settings settings;
	settings.InitialSize = 10; 
	settings.BurstSizeMin = 5;
	settings.BurstSizeMax = 10;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.2f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = Vector(-20.0f, 0.0f, 0.0f) + GameManagerSystem::RandomVector(0.1f, 0.1f);
		Vector accel = GameManagerSystem::RandomVector(0.1f, 1.0f);
		p->Acceleration = Vector(1.0f * accel.X, 0.05f * accel.Y, 0.05f * accel.Z);
		p->LifeTime = Random(0.25f, 0.5f);
		p->Scale = Vector(1.0f, 1.0f, 1.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Position += p->Acceleration;
		p->Scale = Vector(1.0f, 1.0f, 1.0f) * (1.0f - Lerp(p->Age/p->LifeTime, 0.1f, 1.0f));
	};
	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings, "Textures/puff_512.png", eResourceSource::GAME);
	ParticleComponent* ParticleComp = world->GetComponent<ParticleComponent>(ParticlesEnt);
	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);
	return ParticleComp;
}

ParticleComponent* GameManagerSystem::SpawnEmitter2(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt2Trans = ParticlesEnt->GetTransform();
	ParticlesEnt2Trans.SetLocalTranslation(Vector(0.0f, 0.0f, -20.0f));
	ParticleEmitter::Settings settings;
	settings.InitialSize = 10;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = Vector(0.0f, 0.0f, -20.0f) + GameManagerSystem::RandomVector(-2.0f, 2.0f);
		p->Acceleration = GameManagerSystem::RandomVector(0.01f, 0.1f);
		p->LifeTime = Random(10.0f, 12.0f);
		p->Scale = Vector(1.0f, 1.0f, 1.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Position += p->Acceleration;
	};
	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	ParticleComponent* ParticleComp = world->GetComponent<ParticleComponent>(ParticlesEnt);
	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);
	return ParticleComp;
}

ParticleComponent* GameManagerSystem::SpawnEmitter3(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = ParticlesEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(-20.0f, 0.0f, 0.0f));
	ParticleEmitter::Settings settings;
	settings.InitialSize = 10;
	settings.BurstSizeMin = 5;
	settings.BurstSizeMax = 10;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.2f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = Vector(0.0f, 0.0f, 0.0f) + GameManagerSystem::RandomVector(0.2f, 0.2f);
		Vector accel = GameManagerSystem::RandomVector(0.1f, 1.0f);
		p->Acceleration = Vector(0.05f * accel.X, 1.0f * accel.Y, 0.05f * accel.Z);
		p->LifeTime = Random(0.25f, 0.5f);
		p->Scale = Vector(1.0f, 1.0f, 1.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Position += p->Acceleration;
		float life = Lerp(p->Age / p->LifeTime, 0.1f, 1.0f);
		p->Scale = Vector(1.0f, 1.0f, 1.0f) * (1.0f - life * life);
	};
	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings, "Textures/puff_512.png", eResourceSource::GAME);
	ParticleComponent* ParticleComp = world->GetComponent<ParticleComponent>(ParticlesEnt);
	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);
	return ParticleComp;
}

ParticleComponent* GameManagerSystem::SpawnEmitter4(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesEnt1Trans = ParticlesEnt->GetTransform();
	ParticlesEnt1Trans.SetLocalTranslation(Vector(-20.0f, 4.0f, 20.0f));
	ParticleEmitter::Settings settings;
	settings.InitialSize = 10;
	settings.BurstSizeMin = 5;
	settings.BurstSizeMax = 10;
	settings.BurstTimeMin = 0.1f;
	settings.BurstTimeMax = 0.2f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = Vector(0.0f, 0.0f, 0.0f) + GameManagerSystem::RandomVector(0.2f, 0.2f);
		Vector accel = GameManagerSystem::RandomVector(0.1f, 1.0f);
		p->Acceleration = Vector(0.1f * accel.X, 1.0f * accel.Y, 0.1f * accel.Z);
		p->LifeTime = Random(0.25f, 0.5f);
		p->Scale = Vector(6.0f, 6.0f, 6.0f);
	};
	settings.ParticleUpdateFunc = [](ParticleEmitter::Particle* p) {
		p->Position += p->Acceleration;
		float life = Lerp(p->Age / p->LifeTime, 0.1f, 1.0f);
		p->Scale = Vector(6.0f, 6.0f, 6.0f) * (1.0f - life * life);
	};
	DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings, "Textures/puff_512.png", eResourceSource::GAME);
	ParticleComponent* ParticleComp = world->GetComponent<ParticleComponent>(ParticlesEnt);
	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);
	return ParticleComp;
}


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
	CreatePointLight(world, 100.0f);

	AddPointLights(world, 7);

	CreateSpotLight(world, 200.0f);


	Entity* Ground = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Ground, "Models/Sponza/sponza.obj", eResourceSource::GAME);
	EntityTransform& groundTrans = Ground->GetTransform();
	MeshRenderingComponent* sponzaMesh = world->GetComponent<MeshRenderingComponent>(Ground);
	for (int i = 0; i < sponzaMesh->GetMesh()->GetSubMeshes().GetSize(); ++i)
	{
		sponzaMesh->SetMaterial(i, PhongMaterial(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f), 8.0f));
	}
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
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Cleanup");
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
	Vector PointLightPos = Vector(Random(-1.0f, 1.0f), Random(0.0f, 0.2f), Random(-0.5f, 0.5f)) * 1000.0f;
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(Random(0.0f, 1.0f), Random(0.0, 0.5f), Random(0.0f, 0.2f));

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
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(Random(0.0f, 1.0f), Random(0.0, 0.5f), Random(0.0f, 0.2f));
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


float GameManagerSystem::Random()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float GameManagerSystem::Random(float min, float max)
{
	float rnd = Random();
	return Lerp(min, max, rnd);
}

Vector GameManagerSystem::RandomVector(float min, float max)
{
	float rndX = GameManagerSystem::Random();
	float rndY = GameManagerSystem::Random();
	float rndZ = GameManagerSystem::Random();
	return Vector(Lerp(min, max, rndX), Lerp(min, max, rndY), Lerp(min, max, rndZ));
}
