#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Core.hpp>
#include <DroneComponent.hpp>
#include <Debugging/DebugDrawComponents.hpp>
#include <Debugging/DebugDrawSystem.hpp>
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
#include <UI/ScreenSpaceTextComponent.hpp>

using namespace Poly;
using namespace AMJ;

void GameManagerSystem::CreateStartScene(World* world)
{
	gConsole.LogInfo("GameManagerSystem::CreateScene");

	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	srand(42);

	gDebugConfig.DisplayFPS = true;

	CreateCamera(world);

	gameMgrCmp->AminModel = CreateModel(world, "Models/Drone/OBJ/Drone_00.obj");
	gameMgrCmp->AnimKeys = LoadAnimTrack(world, "Animations/cube0.x");
	gameMgrCmp->GameEntities.PushBack(gameMgrCmp->AminModel);

	Matrix scale;	
	scale.SetScale(Vector::ONE * 500.0f);
	Matrix move;
	move.SetTranslation(Vector::UNIT_Y * 500.0f);
	gameMgrCmp->ShapeTransform = move * scale;
	gameMgrCmp->AnimShape_cube = LoadAnimShape(world, "Shapes/sh_cube.obj");
	gameMgrCmp->AnimShape_torus = LoadAnimShape(world, "Shapes/sh_torus.obj");

	CreateDrones(world, Vector(-400.0f, 0.0f, -300.0f));

	CreateLevel(world);

	CreateTextUI(world);

	// gDebugConfig.DisplayFPS = true;
	// gDebugConfig.DebugRender = true;
}

AnimKeys GameManagerSystem::LoadAnimTrack(World* world, String path)
{
	gConsole.LogInfo("GameManagerSystem::LoadAnimTrack");
	String animSrc = LoadTextFileRelative(eResourceSource::GAME, path);
	// gConsole.LogInfo("GameManagerSystem::CreateAnimTrack AnimSrc: {}", animSrc);

	Dynarray<Vector> positions;
	Dynarray<Vector> scales;
	Dynarray<Quaternion> rotations;

	bool hasAnimationKey = false;
	bool hasAnimationKeyRotation = false;
	bool hasAnimationKeyScale = false;
	bool hasAnimationKeyPosition = false;
	int rowCounterRotation = 0;
	int rowCounterScale = 0;
	int rowCounterPosition = 0;

	animSrc.Replace("\r\n", "\n");
	animSrc.GetTrimmed();
	Dynarray<String> rows = animSrc.Split('\n');
	for (String row : rows)
	{
		if (row.IsEmpty())
		{
			continue;
		}

		if (row.Contains("AnimationKey"))
		{
			hasAnimationKey = true;
		}

		if (row.Contains("Rotation"))
		{
			hasAnimationKeyRotation = true;
		}

		if (row.Contains("Scale"))
		{
			hasAnimationKeyScale = true;
		}

		if (row.Contains("Position"))
		{
			hasAnimationKeyPosition = true;
		}

		if (row.Contains("}"))
		{
			hasAnimationKey = false;
			hasAnimationKeyRotation = false;
			hasAnimationKeyScale = false;
			hasAnimationKeyPosition = false;
		}

		if (hasAnimationKey)
		{
			// gConsole.LogInfo("GameManagerSystem::CreateAnimTrack key: {}, R: {}, S: {}, T: {}, row: {}",
			// 	hasAnimationKey, hasAnimationKeyRotation, hasAnimationKeyScale, hasAnimationKeyPosition, row);

			if (hasAnimationKeyRotation)
			{
				if (rowCounterRotation == 1)
				{
					gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: read if rotation or track ID?, row: {}", row);
				}

				if (rowCounterRotation == 2)
				{
					gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: number of keyframes, row: {}", row);
				}

				if (rowCounterRotation > 2)
				{
					rotations.PushBack(AnimTrack_ReadQuternion4FromRow(row));
				}

				rowCounterRotation++;
			}

			if (hasAnimationKeyScale)
			{
				if (rowCounterScale == 1)
				{
					gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: read if rotation or track ID?, row: {}", row);
				}

				if (rowCounterScale == 2)
				{
					gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: number of keyframes, row: {}", row);
				}

				if (rowCounterScale > 2)
				{
					scales.PushBack(AnimTrack_ReadVector3FromRow(row));
				}

				rowCounterScale++;
			}

			if (hasAnimationKeyPosition)
			{
				if (rowCounterPosition == 1)
				{
					gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: read if rotation or track ID?, row: {}", row);
				}

				if (rowCounterPosition == 2)
				{
					gConsole.LogInfo("GameManagerSystem::CreateAnimTrack TODO: number of keyframes, row: {}", row);
				}

				if (rowCounterPosition > 2)
				{
					positions.PushBack(AnimTrack_ReadVector3FromRow(row));
				}

				rowCounterPosition++;
			}
		}
	}

	gConsole.LogInfo("GameManagerSystem::CreateAnimTrack Print loaded rotations:");
	for (size_t i = 0; i < rotations.GetSize(); ++i)
	{
		gConsole.LogInfo("GameManagerSystem::CreateAnimTrack rotation[{}]: {}", i, rotations[i]);
	}

	gConsole.LogInfo("GameManagerSystem::CreateAnimTrack Print loaded scales:");
	for (size_t i = 0; i < scales.GetSize(); ++i)
	{
		gConsole.LogInfo("GameManagerSystem::CreateAnimTrack scale[{}]: {}", i, scales[i]);
	}

	gConsole.LogInfo("GameManagerSystem::CreateAnimTrack Print loaded positions:");
	for (size_t i = 0; i < positions.GetSize(); ++i)
	{
		gConsole.LogInfo("GameManagerSystem::CreateAnimTrack position[{}]: {}", i, positions[i]);
	}

	// ASSERTE(false, "Stop at AnimTrack loading for easier debugging ;)");

	return AnimKeys(positions, rotations, scales);
}

Vector GameManagerSystem::AnimTrack_ReadVector3FromRow(String row)
{
	// 0;4;0.000000, 0.000000, 0.000000, 0.000000;;,	
	Dynarray<String> tokens = row.Split(';'); // values are at token with index 2 
	Dynarray<String> channels = tokens[2].Split(',');
	float x = std::atof(channels[0].GetCStr());
	float y = std::atof(channels[1].GetCStr());
	float z = std::atof(channels[2].GetCStr());
	return Vector(x, y, z);
}

Quaternion GameManagerSystem::AnimTrack_ReadQuternion4FromRow(String row)
{
	// 0;4;0.000000, 0.000000, 0.000000, 0.000000;;,
	Dynarray<String> tokens = row.Split(';'); // values are at token with index 2 
	Dynarray<String> channels = tokens[2].Split(',');
	float x = std::atof(channels[0].GetCStr());
	float y = std::atof(channels[1].GetCStr());
	float z = std::atof(channels[2].GetCStr());
	float w = std::atof(channels[3].GetCStr());
	return Quaternion(x, y, z, w);
}

AnimShape GameManagerSystem::LoadAnimShape(World* world, String path)
{
	gConsole.LogInfo("GameManagerSystem::LoadAnimShape");
	String animSrc = LoadTextFileRelative(eResourceSource::GAME, path);
	// gConsole.LogInfo("GameManagerSystem::CreateAnimTrack AnimSrc: {}", animSrc);

	Dynarray<Vector> positions;

	animSrc.Replace("\r\n", "\n");
	animSrc.GetTrimmed();
	Dynarray<String> rows = animSrc.Split('\n');
	for (String row : rows)
	{
		if (row.IsEmpty())
		{
			continue;
		}

		if (row.Contains("v"))
		{
			positions.PushBack(AnimShape_ReadVector3FromRow(row));
		}
	}

	gConsole.LogInfo("GameManagerSystem::LoadAnimShape Print loaded rotations:");
	for (size_t i = 0; i < positions.GetSize(); ++i)
	{
		gConsole.LogInfo("GameManagerSystem::CreateAnimShape positions[{}]: {}", i, positions[i]);
	}

	return AnimShape(positions, Random());
}

Vector GameManagerSystem::AnimShape_ReadVector3FromRow(String row)
{
	//v -1.000000 0.000000 1.000000
	Dynarray<String> tokens = row.Split(' ');
	float x = std::atof(tokens[1].GetTrimmed().GetCStr());
	float y = std::atof(tokens[2].GetTrimmed().GetCStr());
	float z = std::atof(tokens[3].GetTrimmed().GetCStr());
	return Vector(x, y, z);
}

void GameManagerSystem::CreateTextUI(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* text = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<ScreenSpaceTextComponent>(
		world,
		text,
		Vector2i(50, 50),
		"Fonts/Raleway/Raleway-Regular.ttf",
		eResourceSource::ENGINE,
		40.0f,
		"powered by PolyEngine"
	);
	gameMgrCmp->GameEntities.PushBack(text);

	Entity* textFPS = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<ScreenSpaceTextComponent>(
		world,
		textFPS,
		Vector2i(50, 25),
		"Fonts/Raleway/Raleway-Regular.ttf",
		eResourceSource::ENGINE,
		40.0f,
		""
	);
	gameMgrCmp->GameEntities.PushBack(textFPS);
	gameMgrCmp->TextFPS = textFPS;
}

Entity* GameManagerSystem::CreateModel(World* world, String path)
{
	Entity* model = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(world, model, eDebugDrawPreset::STATIC);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, model, path, eResourceSource::GAME);
	return model;
}

void GameManagerSystem::CreateCamera(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	CameraComponent* cameraCmp = DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, camera, 90_deg, 1.0f, 20000.f);
	// cameraCmp->SetRenderingMode(eRenderingModeType::IMMEDIATE_DEBUG);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, camera);
	// gameMgrCmp->PostCmp->Exposure = 0.5f; // night
	gameMgrCmp->PostCmp->Exposure = 10.0f; // day

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-200.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(camera));
	gameMgrCmp->Camera = camera;
	gameMgrCmp->GameEntities.PushBack(camera);

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, keyDirLight, Color(1.0f, 1.0f, 1.0f), 5.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);

}

void GameManagerSystem::CreateDrones(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	for (int z = 0; z < 20; ++z)
	{
		for (int y = 0; y < 20; ++y)
		{
			Entity* droneRoot = DeferredTaskSystem::SpawnEntityImmediate(world);

			Entity* droneLOD0 = DeferredTaskSystem::SpawnEntityImmediate(world);
			droneLOD0->SetParent(droneRoot);
			MeshRenderingComponent* LOD0 = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, droneLOD0, "Models/Drone/OBJ/Drone_00.obj", eResourceSource::GAME);
			size_t LOD0MaterialsLen = LOD0->GetMesh()->GetSubMeshes().GetSize();
			for (size_t i = 0; i < LOD0MaterialsLen; ++i)
			{
				LOD0->SetMaterial(i, Material(Color::WHITE * 1000.0f, Color::WHITE, 1.0f, 1.0f, 0.5f));
			}

			Entity* droneLOD1 = DeferredTaskSystem::SpawnEntityImmediate(world);
			droneLOD1->SetParent(droneRoot);
			MeshRenderingComponent* LOD1 = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, droneLOD1, "Models/Drone/OBJ/Drone_01.obj", eResourceSource::GAME);
			size_t LOD1MaterialsLen = LOD1->GetMesh()->GetSubMeshes().GetSize();
			for (size_t i = 0; i < LOD1MaterialsLen; ++i)
			{
				LOD1->SetMaterial(i, Material(Color::WHITE * 1000.0f, Color::WHITE, 1.0f, 0.5f, 0.5f));
			}

			Entity* droneLOD2 = DeferredTaskSystem::SpawnEntityImmediate(world);
			droneLOD2->SetParent(droneRoot);
			MeshRenderingComponent* LOD2 = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, droneLOD2, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
			LOD2->GetTransform().SetLocalScale(Vector::ONE * 0.5f);
			size_t LOD2MaterialsLen = LOD2->GetMesh()->GetSubMeshes().GetSize();
			for (size_t i = 0; i < LOD2MaterialsLen; ++i)
			{
				LOD2->SetMaterial(i, Material(Color::WHITE * 1000.0f, Color::WHITE, 1.0f, 0.01f, 0.5f));
			}

			Entity* droneLight = DeferredTaskSystem::SpawnEntityImmediate(world);
			droneLight->SetParent(droneRoot);
			float range = Lerp(100.0f, 200.0f, Random());
			PointLightComponent* pointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, droneLight, Color::WHITE, 0.0f, range);
			droneLight->GetTransform().SetLocalTranslation(Vector::UNIT_Y * 10.0f);


			EntityTransform& droneTrans = droneRoot->GetTransform();
			Vector rndPos = RandomVector() * 2.0f - 1.0f;
			Vector randomOffset = Vector(rndPos.X * 1.0f, rndPos.Y * 0.1f, rndPos.Z * 1.0f) * 5.0f;
			droneTrans.SetGlobalTranslation(pos + Vector(100.0f * y, 0.0f, 100.0f * z) + randomOffset + Vector::UNIT_Y * 20.0f);
			Vector rndRot = RandomVector() * 2.0f - 1.0f;
			float rotMax = 5.0f;
			Quaternion rotOffset = Quaternion(Vector::UNIT_X, 1.0_deg * rndRot.X * rotMax) * Quaternion(Vector::UNIT_Y, 1.0_deg * rndRot.Y * rotMax) * Quaternion(Vector::UNIT_Z, 1.0_deg * rndRot.Z * rotMax);
			droneTrans.SetGlobalRotation(rotOffset);
			droneTrans.SetLocalScale(Vector::ONE * 10.0f);

			DroneComponent* droneCmp = DeferredTaskSystem::AddComponentImmediate<DroneComponent>(world, droneRoot);
			droneCmp->InitPosition = droneRoot->GetTransform().GetGlobalTranslation();
			droneCmp->InitScale = droneRoot->GetTransform().GetGlobalScale();
			droneCmp->InitRotation = droneRoot->GetTransform().GetGlobalRotation();
			droneCmp->RestPostion = (RandomVector() * 2.0f - 1.0f);
			droneCmp->LOD0 = LOD0;
			droneCmp->LOD1 = LOD1;
			droneCmp->LOD2 = LOD2;
			droneCmp->LOD1threshold = 500.0f;
			droneCmp->LOD2threshold = 1000.0f;
			droneCmp->Light = pointLightCmp;
			droneCmp->random = Random();

			gameMgrCmp->Drones.PushBack(droneCmp);
			gameMgrCmp->GameEntities.PushBack(droneRoot);
		}
	}
}

void GameManagerSystem::CreateLevel(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	gameMgrCmp->Level = CreateModel(world, "Models/Level2/model_lowpoly.obj");
	gameMgrCmp->Level->GetTransform().SetGlobalScale(Vector::ONE * 2000.0f);
	gameMgrCmp->Level->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	gameMgrCmp->GameEntities.PushBack(gameMgrCmp->Level);

	Entity* meterCube = CreateModel(world, "Models/Primitives/Cube_lowpoly.obj");
	meterCube->GetTransform().SetGlobalTranslation(Vector::UNIT_Z * 1000.0f);
	meterCube->GetTransform().SetGlobalScale(Vector::ONE * 100.0f);
	gameMgrCmp->GameEntities.PushBack(meterCube);
}

void GameManagerSystem::CreatePBRShpereGrid(World* world, Vector pos, Color albedo)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	// Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	// DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, camera, 50_deg, 1.0f, 5000.f);
	// DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 10.0f, 0.003f);
	// DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(world, camera);
	// EntityTransform& cameraTrans = camera->GetTransform();
	// cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	// cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	// world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(camera));

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, keyDirLight, Color(1.0f, 1.0f, 1.0f), 1.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);
	// Entity* fillDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, fillDirLight, Color(0.75f, 0.95f, 1.0f), 0.1f);
	// fillDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg + 180_deg) * Quaternion(Vector::UNIT_X, 65_deg + 180_deg));
	// gameMgrCmp->GameEntities.PushBack(fillDirLight);

	// Entity* ground = DeferredTaskSystem::SpawnEntityImmediate(world);
	// EntityTransform& groundTrans = ground->GetTransform();
	// groundTrans.SetGlobalTranslation(Vector(0.0f, 0.0f, 0.0f));	
	// MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, ground, "Models/Ground/Ground.fbx", eResourceSource::GAME);
	// meshCmp->SetShadingModel(eShadingModel::PBR);
	// int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
	// for (int i = 0; i < materialsNum; ++i)
	// {
	// 	meshCmp->SetPBRMaterial(i, material);
	// }
	// gameMgrCmp->GameEntities.PushBack(ground);

	for (int z = 0; z < 5; ++z)
	{
		for (int y = 0; y < 5; ++y)
		{
			Entity* sphere = DeferredTaskSystem::SpawnEntityImmediate(world);
			EntityTransform& sphereTrans = sphere->GetTransform();
			sphereTrans.SetGlobalTranslation(pos + Vector(50.0f * y, 0.0f, 50.0f * z) - (Vector(50.0f * 5.0f, -100.0f, 50.0f * 5.0f) * 0.5f));
			sphereTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f) * 20.0f);
			sphereTrans.SetLocalRotation(Quaternion(Vector::UNIT_Z, 90.0_deg));
			MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, sphere, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
			int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
			for (int i = 0; i < materialsNum; ++i)
			{
				float Roughness = ((0.01f + z) / 5.0f);
				float Metallic = ((0.01f + y) / 5.0f);
				meshCmp->SetMaterial(i, Material(
					Color(0.0f, 0.0f, 0.0f, 0.0f),
					albedo,
					Roughness,
					Metallic,
					0.5f
				));
			}

			gameMgrCmp->GameEntities.PushBack(sphere);
		}
	}
}

void GameManagerSystem::CreateTranslucent(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Material material(
		Color::BLACK,
		Color(1.0f, 0.5f, 0.5f, 0.75f),
		0.01f,
		0.95f,
		0.5f
	);

	for (int i = 0; i < 8; ++i)
	{
		Entity* translucent = DeferredTaskSystem::SpawnEntityImmediate(world);
		EntityTransform& translucentTrans = translucent->GetTransform();
		Vector randomOffset = RandomVectorRange(-1.0f, 1.0f) * 200.0f;
		translucentTrans.SetGlobalTranslation(Vector(0.0f, 200.0f, 0.0f) + randomOffset);
		translucentTrans.SetLocalScale(Vector(100.0f, 100.0f, 100.0f));
		MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, translucent, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
		meshCmp->SetBlendingMode(eBlendingMode::TRANSLUCENT);
		int materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
		for (int i = 0; i < materialsNum; ++i)
		{
			meshCmp->SetMaterial(i, material);
		}

		gameMgrCmp->GameEntities.PushBack(translucent);
	}
}

void GameManagerSystem::CreateSponza(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	// Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	// CameraComponent* cameraCmp = DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, camera, 50_deg, 1.0f, 5000.f);
	// cameraCmp->SetRenderingMode(eRenderingModeType::IMMEDIATE_DEBUG);
	// DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 10.0f, 0.003f);
	// EntityTransform& cameraTrans = camera->GetTransform();
	// cameraTrans.SetGlobalTranslation(Vector(800.0f, 180.0f, 0.0f));
	// cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));
	// world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(camera));

	// Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	// DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(world, keyDirLight, Color(0.1f, 0.095f, 0.075f), 1.0f);
	// EntityTransform& dirLightTrans = keyDirLight->GetTransform();
	// dirLightTrans.SetLocalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, 65_deg));
	// gameMgrCmp->GameEntities.PushBack(keyDirLight);

	Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);

	gameMgrCmp->GameEntities.PushBack(sponza);
}

void GameManagerSystem::CreateSponzaSample(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(world, camera, 40_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(world, camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(300.0f, 0.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));

	world->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, world->GetComponent<CameraComponent>(camera));

	Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, sponza, "Models/Sponza/sponza_lion.obj", eResourceSource::GAME);
	gameMgrCmp->GameEntities.PushBack(sponza);

	Vector lightPosition = Vector(50.0f, 0.0f, 50.0f);
	Entity* lightEntity = CreatePointLight(world, lightPosition, 100.0f);
	gameMgrCmp->LightsStartPositions.PushBack(lightPosition);
	gameMgrCmp->PointLightEntities.PushBack(lightEntity);
}

void GameManagerSystem::Update(World* world)
{
	// UpdateParticles(world);

	// UpdateLights(world);

	// UpdateModel(world);

	// Vector offset = Vector(800.0f, 0.0f, 0.0f);
	// DebugDrawSystem::DrawLine(world, offset, offset + Vector::UNIT_Y * 1000.0f, Color::RED);
	// DebugDrawSystem::DrawBox(world, offset + Vector(-100.0f, 0.0f, -100.0f), offset + Vector(100.0f, 200.0f, 100.0f), Color::RED);
	// DebugDrawSystem::DrawSphere(world, offset + Vector(-100.0f, 0.0f, -100.0f), 100.0f, Color::RED);

	// UpdatePostProcess(world);

	DebugWorldComponent* com = world->GetWorldComponent<DebugWorldComponent>();
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	gameMgrCmp->TextFPS->GetComponent<ScreenSpaceTextComponent>()->SetText(String::From(com->GetFPSData().LastFPS));

	UpdateAnimTracks(world);

	UpdateAnimDayNight(world);

	UpdateAnimDrones(world);

	UpdateDrones(world);
}

void GameManagerSystem::UpdatePostProcess(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	time *= 0.1f;
	// float exposure = 0.25f * floor(10.0f*4.0f * (time - floor(time)));
	float exposure = 0.25f + 10.0f * time;
	gameMgrCmp->PostCmp->Exposure = exposure;
	gConsole.LogInfo("GameManagerSystem::UpdatePostProcess exposure: {}", gameMgrCmp->PostCmp->Exposure);
}

void GameManagerSystem::UpdateAnimTracks(World* world)
{
	// gConsole.LogInfo("GameManagerSystem::UpdateAnimTracks");

	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float delta = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	float animTime = gameMgrCmp->AnimProgress;
	AnimKeys animKeys = gameMgrCmp->AnimKeys;
	size_t animKeysSize = animKeys.Positions.GetSize();

	animTime += delta;
	animTime = fmod(animTime, 4.0f);
	// gConsole.LogInfo("GameManagerSystem::UpdateAnimTracks AnimProgress: {}", animTime);

	float keyDelta = animTime - floorf(animTime); // fract actually but simpler
	size_t keyPrev = floorf(animTime);
	size_t keyNext = keyPrev + 1 >= animKeysSize ? 0 : keyPrev + 1;

	float keyDeltaSmooth = SmoothStep(0.0f, 1.0f, keyDelta);
	Vector position = Lerp(animKeys.Positions[keyPrev], animKeys.Positions[keyNext], keyDeltaSmooth);
	Vector scale = Lerp(animKeys.Scales[keyPrev], animKeys.Scales[keyNext], keyDeltaSmooth);
	Quaternion rotation = Quaternion::Slerp(animKeys.Rotations[keyPrev], animKeys.Rotations[keyNext], keyDeltaSmooth);

	gameMgrCmp->AminModel->GetTransform().SetGlobalTranslation(position);
	gameMgrCmp->AminModel->GetTransform().SetGlobalScale(scale);
	gameMgrCmp->AminModel->GetTransform().SetGlobalRotation(rotation);

	gameMgrCmp->AnimProgress = animTime;
}

void GameManagerSystem::UpdateAnimDayNight(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float anim = SmoothStep(-0.5f, 0.5f, Sin(20.0_deg * time));

	// 0 - night, 1 - day
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	gameMgrCmp->PostCmp->Exposure = Lerp(1.0f, 10.0f, anim);

	float intensityNight = 100.0f * 1000.0f;
	float animSmooth = SmoothStep(0.0f, 1.0f, anim);
	// float animSmooth2 = SmoothStep(0.0f, 0.5f, anim);
	// float intensity = Lerp(intensityNight, 0.0f, animSmooth2);

	SkyboxWorldComponent* skyboxCmp = world->GetWorldComponent<SkyboxWorldComponent>();
	skyboxCmp->SetTint(Color::WHITE * Lerp(0.2f, 1.0f, animSmooth));

	// gConsole.LogInfo("GameManagerSystem::UpdateAnimDayNight time: {}, anim: {}", time, anim);

	for (auto drone : gameMgrCmp->Drones)
	{
		float animSmooth2 = SmoothStep(0.0f, 0.5f - Lerp(0.0f, 0.2f, drone->random), anim);
		float intensity = Lerp(intensityNight, 0.0f, animSmooth2);
		drone->Light->SetIntensity(intensity);

		Color emissive = Color::WHITE * Lerp(1000.0f, 0.0f, animSmooth2);
		Color albedo = Color::WHITE * Lerp(0.1f, 1.0f, animSmooth2);
		float roughness = Lerp(0.95f, 0.1f, animSmooth2);
		float metallic = Lerp(0.95f, 0.1f, animSmooth2);
		size_t materials = drone->LOD0->GetMesh()->GetSubMeshes().GetSize();
		for (size_t i = 0; i < materials; ++i)
		{
			drone->LOD0->SetMaterial(i, Material(emissive, albedo, roughness, metallic, 0.5f));
		}
	}
}

void GameManagerSystem::UpdateAnimDrones(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float anim = SmoothStep(-0.5f, 0.5f, Sin(20.0_deg * time));
	float animSmooth = SmoothStep(0.0f, 1.0f, anim);

	// gConsole.LogInfo("GameManagerSystem::UpdateAnimDrones time: {}, anim: {}", time);
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	Matrix shapeTransform = gameMgrCmp->ShapeTransform;

	size_t dronesSize = gameMgrCmp->Drones.GetSize();
	size_t shape0Size = gameMgrCmp->AnimShape_cube.ShapeSize;
	size_t shape1Size = gameMgrCmp->AnimShape_torus.ShapeSize;
	// gConsole.LogInfo("GameManagerSystem::UpdateAnimDrones droneSize: {}, shape0Size: {}, shape1Size: {}",
	// 	dronesSize, shape0Size, shape1Size);
	size_t count = std::min(dronesSize, std::max(shape0Size, shape1Size));
	for (size_t i = 0; i < count; ++i)
	{
		DroneComponent* drone = gameMgrCmp->Drones[i];
		Vector pos0 = (i < shape0Size) ? gameMgrCmp->AnimShape_cube.Positions[i] : drone->RestPostion;
		Vector pos1 = (i < shape1Size) ? gameMgrCmp->AnimShape_torus.Positions[i] : drone->RestPostion;
		Vector posAnim = shapeTransform * Lerp(pos0, pos1, 1.0f - animSmooth);
		drone->GetOwner()->GetTransform().SetGlobalTranslation(posAnim);
	}
}

void GameManagerSystem::UpdateDrones(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	Entity* camera = gameMgrCmp->Camera.Get();
	Vector cameraPos = camera->GetTransform().GetGlobalTranslation();
	for (auto drone : gameMgrCmp->Drones)
	{
		Vector dronePos = drone->GetTransform().GetGlobalTranslation();
		float distToCamera = (dronePos - cameraPos).Length();
		if (Abs(distToCamera - drone->LastDistToCamera) > 1.0f )
		{
			if (distToCamera < drone->LOD1threshold) 
			{
				drone->LOD0->SetShadingModel(eShadingMode::PBR);
				drone->LOD1->SetShadingModel(eShadingMode::NONE);
				drone->LOD2->SetShadingModel(eShadingMode::NONE);
			}
			else if (distToCamera > drone->LOD1threshold && distToCamera < drone->LOD2threshold) 
			{
				drone->LOD0->SetShadingModel(eShadingMode::NONE);
				drone->LOD1->SetShadingModel(eShadingMode::PBR);
				drone->LOD2->SetShadingModel(eShadingMode::NONE);
			}
			else if (distToCamera > drone->LOD2threshold)
			{
				drone->LOD0->SetShadingModel(eShadingMode::NONE);
				drone->LOD1->SetShadingModel(eShadingMode::NONE);
				drone->LOD2->SetShadingModel(eShadingMode::PBR);
			}
		}
		drone->LastDistToCamera = distToCamera;
	}
}

void GameManagerSystem::UpdateParticles(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	// float deltaTime = (float)(TimeSystem::GetTimerDeltaTime(world, Poly::eEngineTimer::GAMEPLAY));

	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Vector translation = Vector::UNIT_Y * (0.2f*Abs(Sin(1.0_rad*3.1415f *time + 0.5_rad)));
	Vector scale = Vector::ONE * (1.0f + 0.1f*(0.5f + 0.5f*Sin(2.0_rad*3.1415f *time + 0.5_rad)));
	Quaternion rotation = Quaternion(Vector::UNIT_Y, 0.5_rad * time);

	if (gameMgrCmp->particleHeart)
	{
		gameMgrCmp->particleHeart->GetTransform().SetGlobalTranslation(translation);
		gameMgrCmp->particleHeart->GetTransform().SetGlobalScale(scale);
		gameMgrCmp->particleHeart->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleHeartImpact0)
	{
		gameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalTranslation(translation);
		gameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalScale(scale);
		gameMgrCmp->particleHeartImpact0->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleHeartImpact1)
	{
		gameMgrCmp->particleHeartImpact1->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleHeartImpact2)
	{
		gameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * 0.2f);
		gameMgrCmp->particleHeartImpact2->GetTransform().SetGlobalRotation(rotation);
	}

	if (gameMgrCmp->particleLocalSpace)
	{
		gameMgrCmp->particleLocalSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time), 0.0f, Sin(100.0_deg * time)) * 6.0f);
	}

	if (gameMgrCmp->particleWorldSpace)
	{
		gameMgrCmp->particleWorldSpace->GetTransform().SetGlobalTranslation(Vector(0.0f, 4.0f, 0.0f) + Vector(Cos(100.0_deg * time + 180.0_deg), 0.0f, Sin(100.0_deg * time + 180.0_deg)) * 6.0f);
	}
}

void GameManagerSystem::UpdateLights(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	for (size_t i = 0; i < gameMgrCmp->LightsStartPositions.GetSize(); ++i)
	{
		Entity* pointLight = gameMgrCmp->PointLightEntities[i];
		Vector startPosition = gameMgrCmp->LightsStartPositions[i];
		Vector offset = Vector::UNIT_Y * Sin(2.0_rad * time + 1.0_rad * startPosition.X) * 100.0f;
		EntityTransform& pointLightTrans = pointLight->GetTransform();
		pointLightTrans.SetGlobalTranslation(startPosition + offset);
	}
}

void GameManagerSystem::UpdateModel(World* world)
{
	float time = (float)(world->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	if (gameMgrCmp->AminModel)
	{
		EntityTransform& modelTrans = gameMgrCmp->AminModel->GetTransform();
		modelTrans.SetLocalRotation(Quaternion(Vector::UNIT_Y, 10.0_deg * time));
	}
}

void GameManagerSystem::Deinit(World* world)
{
	gConsole.LogInfo("GameManagerSystem::Deinit");
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	for (auto e : gameMgrCmp->GameEntities)
	{
		DeferredTaskSystem::DestroyEntity(world, e.Get());
	}
}

void GameManagerSystem::CreatePointLights(World* world, int quota)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	gameMgrCmp->LightsStartPositions.Reserve(quota);

	for (int i = 0; i < quota; ++i)
	{
		Vector position = Vector(RandomRange(-1.0f, 1.0f)*1000.0f, RandomRange(0.0f, 800.0f), RandomRange(-1.0f, 1.0f)*500.0f);
		float rangeRnd = pow(RandomRange(0.0f, 1.0f), 8.0f);
		float range = 100.0f + rangeRnd * 2000.0f;
		Entity* lightEntity = CreatePointLight(world, position, range);

		gameMgrCmp->LightsStartPositions.PushBack(position);
		gameMgrCmp->PointLightEntities.PushBack(lightEntity);

		// gConsole.LogInfo("GameManagerSystem::CreateTestLights #{}: Positon: {}, Radius: {}",
		// 	i, position, Range);
	}

	for (size_t i = 0; i < gameMgrCmp->PointLightEntities.GetSize(); ++i)
	{
		gameMgrCmp->GameEntities.PushBack(gameMgrCmp->PointLightEntities[i]);
	}
}

Entity* GameManagerSystem::CreatePointLight(World* world, Vector& position, float Range)
{
	Color lightColor = Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f));

	Entity* pointLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	PointLightComponent* pointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(world, pointLight, lightColor, 100.0f * 1000.0f, Range);
	MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, pointLight, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	meshCmp->SetMaterial(0, Material(Color(0.0f, 0.0f, 0.0f, 0.0f), lightColor, 0.0f, 1.0f, 0.5f));
	EntityTransform& pointLightTrans = pointLight->GetTransform();
	pointLightTrans.SetGlobalScale(Vector::ONE * 5.0f);
	pointLightTrans.SetGlobalTranslation(position);

	return pointLight;
}

void GameManagerSystem::CreateSpotLight(World* world, float Range)
{
	Vector spotLightPos = Vector(50.0f, 50.0f, 0.0f);
	Color lightColor = Color(1.0f, 0.5f, 0.0f) + Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 0.5f), RandomRange(0.0f, 0.2f));
	Quaternion spotLightRot = Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -35_deg);
	// float pointLightRange = 100.0f;
	Entity* spotLight = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<SpotLightComponent>(world, spotLight, lightColor, 1.0f, Range, 5.0f, 17.0f);
	EntityTransform& spotLightTrans = spotLight->GetTransform();
	spotLightTrans.SetLocalTranslation(spotLightPos);
	spotLightTrans.SetLocalRotation(spotLightRot);

	Entity* spotLightDebugSource = DeferredTaskSystem::SpawnEntityImmediate(world);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, spotLightDebugSource, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	MeshRenderingComponent* spotLightMesh = world->GetComponent<MeshRenderingComponent>(spotLightDebugSource);
	spotLightMesh->SetMaterial(0, Material(lightColor, lightColor, 1.0f, 1.0f, 0.5f));
	EntityTransform& spotLightDebugSourceTrans = spotLightDebugSource->GetTransform();
	spotLightDebugSource->SetParent(spotLight);
	spotLightDebugSourceTrans.SetLocalScale(2.0f);
	spotLightDebugSourceTrans.SetLocalTranslation(Vector(0.0f, 0.0f, 0.0f));
}

void GameManagerSystem::SpawnParticles(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();
	Vector particlesPosition = Vector(0.0f, 0.0f, 0.0f);
	gameMgrCmp->particleDefault = SpawnEmitterDefault(world, particlesPosition);
	gameMgrCmp->particleWorldSpace = SpawnEmitterWorldSpace(world, particlesPosition);
	gameMgrCmp->particleLocalSpace = SpawnEmitterLocalSpace(world, particlesPosition);
	gameMgrCmp->particleAmbient = SpawnEmitterAmbient(world, particlesPosition);
	gameMgrCmp->particleAmbientWind = SpawnEmitterAmbientWind(world, particlesPosition);

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
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* shaderball = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& shaderballTrans = shaderball->GetTransform();
	shaderballTrans.SetLocalTranslation(Vector(0.0f, 5.0f, 0.0f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(world, shaderball, "Models/shaderball/PolyEngine_shaderball.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ballMesh = world->GetComponent<MeshRenderingComponent>(shaderball);
	ballMesh->SetMaterial(0, Material(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), 1.0f, 1.0f, 0.5f));
	ballMesh->SetMaterial(1, Material(Color(1.0f, 1.0f, 1.0f), Color(0.4f, 0.4f, 0.4f), 1.0f, 1.0f, 0.5f));
	shaderballTrans.SetLocalScale(0.1f);
	gameMgrCmp->GameEntities.PushBack(shaderball);
}


// #pragma region Spritesheet examples

void GameManagerSystem::SpawnSpritesheet11(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(1.0f, 1.0f);
	settings.SpritePath = "Textures/test_1_1.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet22(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(2.0f, 2.0f);
	settings.SpritePath = "Textures/test_2_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.SpriteColor = Color(0.0f, 1.0f, 0.0f, 0.5f);
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet42(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 2.0f);
	settings.SpritePath = "Textures/test_4_2.png";
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet41(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 1.0f);
	settings.SpritePath = "Textures/test_4_1.png";
	settings.SpriteColor = Color::RED;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheet44Random(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/test_4_4.png";
	settings.IsRandomStartFrame = true;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

void GameManagerSystem::SpawnSpritesheetGandalf(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* spriteSheetEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& SpriteSheetTrans = spriteSheetEnt->GetTransform();
	SpriteSheetTrans.SetLocalTranslation(pos);
	SpriteSheetTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f));
	SpritesheetSettings settings;
	settings.SubImages = Vector2f(4.0f, 4.0f);
	settings.SpritePath = "Textures/gandalf_anim.png";
	settings.Speed = 0.2f;
	DeferredTaskSystem::AddComponentImmediate<SpritesheetComponent>(world, spriteSheetEnt, settings);
	gameMgrCmp->GameEntities.PushBack(spriteSheetEnt);
}

// #pragma endregion


// #pragma region Particle Examples

ParticleComponent* GameManagerSystem::SpawnEmitterDefault(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/none.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.BurstSizeMin = 2;
	settings.BurstSizeMax = 5;
	settings.BurstTimeMax = 1.0f;
	settings.BurstTimeMin = 5.0f;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-10.0f, 10.0f);
		p->Velocity = Vector(0.0f, 0.0f, 0.0f);
		p->LifeTime = RandomRange(1.0f, 1.0f);
		p->Scale = Vector::ONE * RandomRange(10.0f, 10.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterWorldSpace(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 50;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::WORLD_SPACE;
	settings.BurstTimeMin = 0.05f;
	settings.BurstTimeMax = 0.20f;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position += RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(0.5f, 2.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterLocalSpace(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(4.0f, 4.0f);
	spriteSettings.SpritePath = "Textures/test_4_4.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 20;
	settings.Spritesheet = spriteSettings;
	settings.SimulationSpace = ParticleEmitter::eSimulationSpace::LOCAL_SPACE;
	settings.ParticleInitFunc = [](ParticleEmitter::Particle* p) {
		p->Position = RandomVectorRange(-1.0f, 1.0f);
		p->Velocity = RandomVectorRange(0.5f, 1.0f) * 0.001f;
		p->LifeTime = RandomRange(1.0f, 5.0f);
		p->Scale = Vector::ONE * RandomRange(1.0f, 2.0f);
	};

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterAmbient(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 500;
	settings.InitialSize = 500;
	settings.Spritesheet = spriteSettings;
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

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterAmbientWind(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	EntityTransform& particlesTrans = particlesEnt->GetTransform();
	particlesTrans.SetLocalTranslation(pos);

	SpritesheetSettings spriteSettings;
	spriteSettings.SubImages = Vector2f(2.0f, 2.0f);
	spriteSettings.SpritePath = "Textures/test_2_2.png";

	ParticleEmitter::Settings settings;
	settings.MaxSize = 200;
	settings.InitialSize = 0;
	settings.Spritesheet = spriteSettings;
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

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeart(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	// EntityTransform& particlesTrans = particlesEnt->GetTransform();
	// particlesTrans.SetLocalTranslation(pos);

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
	settings.Spritesheet = spriteSettings;
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

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeartImpact(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	// EntityTransform& particlesTrans = particlesEnt->GetTransform();
	// particlesTrans.SetLocalTranslation(pos);

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
	settings.Spritesheet = spriteSettings;
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

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

ParticleComponent* GameManagerSystem::SpawnEmitterHeartImpact2(World* world, Vector pos)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	Entity* particlesEnt = DeferredTaskSystem::SpawnEntityImmediate(world);
	// EntityTransform& particlesTrans = particlesEnt->GetTransform();
	// particlesTrans.SetLocalTranslation(pos);

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
	settings.Spritesheet = spriteSettings;
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

	gameMgrCmp->GameEntities.PushBack(particlesEnt);

	ParticleComponent* particleCmp = DeferredTaskSystem::AddComponentImmediate<ParticleComponent>(world, particlesEnt, settings);
	return particleCmp;
}

void GameManagerSystem::SpawnHeartSystem(World* world)
{
	GameManagerWorldComponent* gameMgrCmp = world->GetWorldComponent<GameManagerWorldComponent>();

	gameMgrCmp->particleHeart = SpawnEmitterHeart(world, Vector(0.0f, 4.0f, 0.0f));
	gameMgrCmp->particleHeartImpact0 = SpawnEmitterHeartImpact(world, Vector(0.0f, 4.0f, 0.0f));
	gameMgrCmp->particleHeartImpact1 = SpawnEmitterHeartImpact(world, Vector(0.0f, 4.0f, 0.0f));
	gameMgrCmp->particleHeartImpact2 = SpawnEmitterHeartImpact2(world, Vector(0.0f, 4.0f, 0.0f));
}

// #pragma endregion