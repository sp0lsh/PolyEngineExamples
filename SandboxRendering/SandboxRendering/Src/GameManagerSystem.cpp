#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <ECS/DeferredTaskSystem.hpp>
#include <Input/InputWorldComponent.hpp>
#include <Movement/FreeFloatMovementComponent.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/SkyboxWorldComponent.hpp>
#include <Rendering/ViewportWorldComponent.hpp>
#include <Rendering/Camera/CameraComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>
#include <Resources/ResourceManager.hpp>
#include <Time/TimeWorldComponent.hpp>

using namespace Poly;

void GameManagerSystem::CreateScene(World* world)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	srand(42);

	// CreateBasic(world);

	CreateSponza(world);

	CreateTranslucent(world);

	SpawnParticles(world);
}

void GameManagerSystem::CreateBasic(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 50_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 10.0f, 0.003f);
	DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, Camera);
	// EntityTransform& cameraTrans = Camera->GetTransform();
	// cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	// cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	// EnumArray<String, eCubemapSide> miramar{
	// 	{ eCubemapSide::RIGHT, "Cubemaps/miramar/miramar_rt.jpg" },
	// 	{ eCubemapSide::LEFT , "Cubemaps/miramar/miramar_lt.jpg" },
	// 	{ eCubemapSide::TOP  , "Cubemaps/miramar/miramar_up.jpg" },
	// 	{ eCubemapSide::DOWN , "Cubemaps/miramar/miramar_dn.jpg" },
	// 	{ eCubemapSide::BACK , "Cubemaps/miramar/miramar_bk.jpg" },
	// 	{ eCubemapSide::FRONT, "Cubemaps/miramar/miramar_ft.jpg" }
	// };
	// DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, miramar);

	Entity* Ground = DeferredTaskSystem::SpawnEntityImmediate(world);
	MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Ground, "Models/Ground/Ground.fbx", eResourceSource::GAME);
	// PhongMaterial material(
	// 	Color(0.0f, 0.0f, 0.0f, 0.0f),
	// 	Color(1.0f, 1.0f, 1.0f, 1.0f),
	// 	Color(1.0f, 1.0f, 1.0f, 1.0f),
	// 	32.0f);
	// int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
	// for (int i = 0; i < materialsNum; ++i)
	// {
	// 	meshCmp->SetMaterial(i, material);
	// }
	GameMgrCmp->GameEntities.PushBack(Ground);
}

void GameManagerSystem::CreateTranslucent(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	PhongMaterial material(
		Color(0.01f, 0.01f, 0.01f, 1.0f),
		Color(1.0f, 0.5f, 0.5f, 0.75f),
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		8.0f
	);

	for (int i = 0; i < 8; ++i)
	{
		Entity* Translucent = DeferredTaskSystem::SpawnEntityImmediate(world);
		EntityTransform& translucentTrans = Translucent->GetTransform();
		Vector randomOffset = RandomVectorRange(-1.0f, 1.0f) * 200.0f;
		translucentTrans.SetGlobalTranslation(Vector(0.0f, 200.0f, 0.0f) + randomOffset);
		translucentTrans.SetLocalScale(Vector(100.0f, 100.0f, 100.0f));
		MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Translucent, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
		int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
		for (int i = 0; i < materialsNum; ++i)
		{
			meshCmp->SetMaterial(i, material);
		}

		GameMgrCmp->GameEntities.PushBack(Translucent);
	}
}

void GameManagerSystem::CreateSponza(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 50_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	PostprocessSettingsComponent* postCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, Camera);
	postCmp->Exposure = 2.0f;

	Entity* KeyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, KeyDirLight, Color(1.0f, 0.95f, 0.75f), 2.0f);
	EntityTransform& dirLightTrans = KeyDirLight->GetTransform();
	dirLightTrans.SetLocalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	GameMgrCmp->GameEntities.PushBack(KeyDirLight);

	EnumArray<String, eCubemapSide> miramar{
		{ eCubemapSide::RIGHT, "Cubemaps/miramar/miramar_rt.jpg" },
		{ eCubemapSide::LEFT , "Cubemaps/miramar/miramar_lt.jpg" },
		{ eCubemapSide::TOP  , "Cubemaps/miramar/miramar_up.jpg" },
		{ eCubemapSide::DOWN , "Cubemaps/miramar/miramar_dn.jpg" },
		{ eCubemapSide::BACK , "Cubemaps/miramar/miramar_bk.jpg" },
		{ eCubemapSide::FRONT, "Cubemaps/miramar/miramar_ft.jpg" }
	};
	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(world, miramar);

	Entity* Sponza = DeferredTaskSystem::SpawnEntityImmediate(world);
	MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);
	PhongMaterial material(
		Color(0.0f, 0.0f, 0.0f, 0.0f),
		Color(1.0f, 1.0f, 1.0f, 1.0f),
		Color(0.1f, 0.1f, 0.1f, 0.0f),
		8.0f);
	int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
	for (int i = 0; i < materialsNum; ++i)
	{
		meshCmp->SetMaterial(i, material);
	}
	GameMgrCmp->GameEntities.PushBack(Sponza);

	CreatePointLights(world, 512);
}

void GameManagerSystem::CreateSponzaSample(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* Camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, Camera, 40_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, Camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = Camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(300.0f, 0.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));

	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(Camera));

	Entity* Sponza = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, Sponza, "Models/Sponza/sponza_lion.obj", eResourceSource::GAME);
	GameMgrCmp->GameEntities.PushBack(Sponza);

	Vector LightPosition = Vector(50.0f, 0.0f, 50.0f);
	Entity* LightEntity = CreatePointLight(world, LightPosition, 100.0f);
	GameMgrCmp->LightsStartPositions.PushBack(LightPosition);
	GameMgrCmp->PointLightEntities.PushBack(LightEntity);
}

void GameManagerSystem::Update(World* world)
{
	float Time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	UpdateParticles(world);

	UpdateLights(world);
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Cleanup");
}

void GameManagerSystem::CreatePointLights(World* world, int quota)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	GameMgrCmp->LightsStartPositions.Reserve(quota);

	for (int i = 0; i < quota; ++i)
	{
		Vector position = Vector(RandomRange(-1.0f, 1.0f)*1000.0f, RandomRange(0.0f, 800.0f), RandomRange(-1.0f, 1.0f)*500.0f);
		Entity* lightEntity = CreatePointLight(world, position, 300.0f);

		GameMgrCmp->LightsStartPositions.PushBack(position);
		GameMgrCmp->PointLightEntities.PushBack(lightEntity);

		// gConsole.LogInfo("GameManagerSystem::CreateTestLights #{}: Positon: {}, Radius: {}",
		// 	i, position, Range);
	}

	for (int i = 0; i < GameMgrCmp->PointLightEntities.GetSize(); ++i)
	{
		GameMgrCmp->GameEntities.PushBack(GameMgrCmp->PointLightEntities[i]);
	}
}

void GameManagerSystem::UpdateLights(World* world)
{
	float Time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (int i = 0; i < GameMgrCmp->LightsStartPositions.GetSize(); ++i)
	{
		Entity* PointLight = GameMgrCmp->PointLightEntities[i];
		Vector StartPosition = GameMgrCmp->LightsStartPositions[i];
		Vector Offset = Vector::UNIT_Y * Sin(2.0_rad * Time + 1.0_rad * StartPosition.X) * 100.0f;
		EntityTransform& PointLightTrans = PointLight->GetTransform();
		PointLightTrans.SetGlobalTranslation(StartPosition + Offset);
	}
}

Entity* GameManagerSystem::CreatePointLight(World* world, Vector& position, float Range)
{
	Color LightColor = Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f));

	Entity* PointLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	PointLightComponent* PointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, PointLight, LightColor, 500.0f, Range);
	MeshRenderingComponent* MeshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, PointLight, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	EntityTransform& PointLightTrans = PointLight->GetTransform();
	PointLightTrans.SetGlobalScale(Vector::ONE * 5.0f);
	PointLightTrans.SetGlobalTranslation(position);

	return PointLight;
}

void GameManagerSystem::CreateSpotLight(World* world, float Range)
{
	Vector SpotLightPos = Vector(50.0f, 50.0f, 0.0f);
	Color LightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 0.5f), RandomRange(0.0f, 0.2f));
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

void GameManagerSystem::UpdateParticles(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Vector translation = Vector::UNIT_Y * (0.2f*Abs(Sin(1.0_rad*3.1415f *time + 0.5_rad)));
	Vector scale = Vector::ONE * (1.0f + 0.1f*(0.5f + 0.5f*Sin(2.0_rad*3.1415f *time + 0.5_rad)));
	Quaternion rotation = Quaternion(Vector::UNIT_Y, 0.5_rad * time);

	if (GameMgrCmp->particleHeart)
	{
		GameMgrCmp->particleHeart->GetTransform().SetGlobalTranslation(translation);
		GameMgrCmp->particleHeart->GetTransform().SetGlobalScale(scale);
		GameMgrCmp->particleHeart->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleHeartImpact0)
	{
		GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalTranslation(translation);
		GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalScale(scale);
		GameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleHeartImpact1)
	{
		GameMgrCmp->particleHeartImpact1->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleHeartImpact2)
	{
		GameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * 0.2f);
		GameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalRotation(rotation);
	}

	if (GameMgrCmp->particleLocalSpace)
	{
		GameMgrCmp->particleLocalSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time), 0.0f, Sin(100.0_deg * time)) * 6.0f);
	}

	if (GameMgrCmp->particleWorldSpace)
	{
		GameMgrCmp->particleWorldSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time + 180.0_deg), 0.0f, Sin(100.0_deg * time + 180.0_deg)) * 6.0f);
	}
}

void GameManagerSystem::SpawnParticles(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	Vector particlesPosition = Vector(0.0f, 0.0f, 0.0f);
	GameMgrCmp->particleDefault = SpawnEmitterDefault(world, particlesPosition);
	GameMgrCmp->particleWorldSpace = SpawnEmitterWorldSpace(world, particlesPosition);
	GameMgrCmp->particleLocalSpace = SpawnEmitterLocalSpace(world, particlesPosition);
	GameMgrCmp->particleAmbient = SpawnEmitterAmbient(world, particlesPosition);
	GameMgrCmp->particleAmbientWind = SpawnEmitterAmbientWind(world, particlesPosition);

	// SpawnHeartSystem(world);
}

void GameManagerSystem::SpawnSpritesSheets(World* world)
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
	settings.SpriteColor = Color(0.0f, 1.0f, 0.0f, 0.5f);
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
	settings.SpriteColor = Color::RED;
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
	settings.IsRandomStartFrame = true;
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


#pragma region Particle Examples

ParticleComponent* GameManagerSystem::SpawnEmitterDefault(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/none.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.BurstSizeMin = 2;
	settings.BurstSizeMax = 5;
	settings.BurstTimeMax = 1.0f;
	settings.BurstTimeMin = 5.0f;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-10.0f, 10.0f);
		p->Velocity = Vector(0.0f, 0.0f, 0.0f);
		p->LifeTime = RandomRange(1.0f, 1.0f);
		p->Scale = Vector::ONE * RandomRange(10.0f, 10.0f);
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
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

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

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
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

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterAmbient(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 500;
	settings.InitialSize = 500;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 10;
	settings.BurstSizeMax = 20;
	settings.BaseColor = Color(1.0f, 1.0f, 1.0f, 0.5f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f) * 10.0f;
		p->Velocity = RandomVectorRange(-1.0f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(5.0f, 10.0f);
		p->Scale = Vector::ONE * RandomRange(0.025f, 0.05f);
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterAmbientWind(World* world, Vector pos)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* ParticlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& ParticlesTrans = ParticlesEnt->GetTransform();
	ParticlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 200;
	settings.InitialSize = 0;
	settings.SpritesheetSettings = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 1.0f;
	settings.BurstTimeMax = 2.0f;
	settings.BurstSizeMin = 200;
	settings.BurstSizeMax = 400;
	settings.BaseColor = Color(1.0f, 1.0f, 1.0f, 0.1f);
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += Vector(-20.0f, 2.0f, 0.0f) + RandomVectorRange(-1.0f, 1.0f) * 10.0f;
		p->Velocity = Vector(RandomRange(0.75f, 1.0f) * 0.5f, 0.0f, 0.0f);
		p->LifeTime = RandomRange(0.75f, 1.0f) * 2.0f;
		p->Scale = Vector::ONE * RandomRange(0.75f, 1.0f) * 4.0f;
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
	settings.BaseColor = Color(1.2f, 0.8f, 0.8f, 0.5f);
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
		rnd.Y = 0.9f*rnd.Y - Abs(rnd.X) * sqrt((20.0f - Abs(rnd.X)) / 15.0f);
		rnd.Y *= -0.8f;
		p->Position += rnd;
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(3.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(0.02f, 0.2f);
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
	settings.BaseColor = Color(1.5f, 1.0f, 1.0f, 0.95f);
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
	settings.BaseColor = Color(2.0f, 0.5f, 0.5f, 0.2f);
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
		p->Scale = Vector::ONE * Lerp(0.4f, 0.001f, pow(p->Age / p->LifeTime, 8.0f));
	};

	GameMgrCmp->GameEntities.PushBack(ParticlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, ParticlesEnt, settings);
	return particleCmp;
}

void GameManagerSystem::SpawnHeartSystem(World* world)
{
	GameManagerWorldComponent* GameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	GameMgrCmp->particleHeart = SpawnEmitterHeart(world, Vector(0.0f, 4.0f, 0.0f));
	GameMgrCmp->particleHeartImpact0 = SpawnEmitterHeartImpact(world, Vector(0.0f, 4.0f, 0.0f));
	GameMgrCmp->particleHeartImpact1 = SpawnEmitterHeartImpact(world, Vector(0.0f, 4.0f, 0.0f));
	GameMgrCmp->particleHeartImpact2 = SpawnEmitterHeartImpact2(world, Vector(0.0f, 4.0f, 0.0f));
}

#pragma endregion