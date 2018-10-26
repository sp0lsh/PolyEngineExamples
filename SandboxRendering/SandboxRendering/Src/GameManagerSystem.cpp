#include "GameManagerSystem.hpp"

#include "GameManagerWorldComponent.hpp"

#include <Math/Random.hpp>
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

void GameManagerSystem::Init(Scene* scene)
{
	gConsole.LogInfo("GameManagerSystem::Init");
	
	srand(42);

	CreateShadowsTestScene(scene);

	// CreateShadingTestScene(scene);
	
	// CreateSponza(scene);
}

void GameManagerSystem::CreateShadingTestScene(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);
	CameraComponent* cameraCmp = DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 35_deg, 1.0f, 5000.f);
	// PostProcessFancyWarm(gameMgrCmp->PostCmp);
	PostProcessFancyCold(gameMgrCmp->PostCmp);
	// PostProcessNone(gameMgrCmp->PostCmp);

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-550.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	gameMgrCmp->Camera = camera;
	
	ViewportWorldComponent* viewport = scene->GetWorldComponent<ViewportWorldComponent>();
	viewport->SetCamera(0, cameraCmp);
	cameraCmp->SetForcedRatio(true);
	cameraCmp->SetAspect(2.4f); // anamorphic 2.40 : 1.0	(2.39)

	// Dynarray<String> hdrs{ "HDR/Barce.hdr", "HDR/Hall.hdr", "HDR/Playa.hdr", "HDR/Path.hdr", "HDR/Factory.hdr" };
	// DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(scene, hdrs, eResourceSource::GAME);
	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(scene, "HDR/HDR.hdr", eResourceSource::GAME);

	CreatePBRShpereGrid(scene, Vector::UNIT_X *  200.0f,	Color::WHITE		);
	CreatePBRShpereGrid(scene, Vector::ZERO,				Color::WHITE * 0.5f	);
	CreatePBRShpereGrid(scene, Vector::UNIT_X * -200.0f,	Color::BLACK		);

	CreatePointLights(scene, 100);

	// Entity* entityPlane = CreateModel(scene, "Models/Primitives/Cube.obj");
	// entityPlane->GetTransform().SetGlobalScale(Vector(5000.0f, 1.0f, 5000.0f));


	// Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);
	// gameMgrCmp->GameEntities.PushBack(sponza);
}

void GameManagerSystem::CreateShadowsTestScene(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	CameraComponent* cameraCmp = DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 35_deg, 1.0f, 10000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);

	cameraCmp->SetRenderingMode(eRenderingModeType::IMMEDIATE_DEBUG);

	// PostProcessFancyCold(gameMgrCmp->PostCmp);
	PostProcessNone(gameMgrCmp->PostCmp);

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-550.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	gameMgrCmp->Camera = camera;
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));

	ViewportWorldComponent* viewportCmp = scene->GetWorldComponent<ViewportWorldComponent>();
	viewportCmp->SetCamera(0, cameraCmp);

	gameMgrCmp->KeyDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, gameMgrCmp->KeyDirLight.Get(), Color(1.0f, 0.8f, 0.8f), 10.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -65_deg));
	// gameMgrCmp->KeyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_X, 85_deg) * Quaternion(Vector::UNIT_Y, -20_deg));
	gameMgrCmp->KeyDirLight->GetTransform().SetGlobalRotation(Quaternion(EulerAngles(85.0_deg, 0.0_deg, 0.0_deg)));
	gameMgrCmp->GameEntities.PushBack(gameMgrCmp->KeyDirLight);

		
	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(scene, "HDR/Path.hdr", eResourceSource::GAME);

	Entity* entityPlane = CreateModel(scene, "Models/Primitives/Cube.obj");
	entityPlane->GetTransform().SetGlobalScale(Vector(4000.0f, 0.1f, 2000.0f));
	entityPlane->GetTransform().SetGlobalTranslation(Vector::UNIT_Y * -100.0f);

	CreateRandomCubes(scene);

	// Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(scene);
	// DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);
	// // sponza->GetTransform().SetGlobalScale(Vector::ONE * 0.5f);
	// sponza->GetTransform().SetGlobalTranslation(Vector::UNIT_X * 1000.0f);
	// gameMgrCmp->GameEntities.PushBack(sponza);
}

void GameManagerSystem::PostProcessFancyCold(PostprocessSettingsComponent* postCmp)
{
	postCmp->Exposure = 2.0f;
	postCmp->DOFSize = 0.1f;
	postCmp->DOFPoint = 700.0f;
	postCmp->DOFRange = 1200.0f;
	postCmp->BloomScale = 0.5f;
	// postCmp->BloomBlurScaleX = 0.5f;
	// postCmp->BloomBlurScaleY = 100.0f;
	postCmp->AbberationScale = 0.5f;
	postCmp->GrainScale = 0.5f;
	postCmp->MotionBlurScale = 0.5f;
	postCmp->VignetteScale = 2.0f;
	// postCmp->FogColor = Color(0.2f, 0.3f, 0.5f) * 0.1f;
	// postCmp->FogDensity = 0.11f;
	// postCmp->Temperature = 12500.0f;
	// postCmp->DOFShow = 1.0f;
}

void GameManagerSystem::PostProcessFancyWarm(PostprocessSettingsComponent* postCmp)
{
	postCmp->Exposure = 2.0f;
	postCmp->DOFSize = 0.1f;
	postCmp->DOFPoint = 700.0f;
	postCmp->DOFRange = 1200.0f;
	postCmp->BloomScale = 0.5f;
	// postCmp->BloomBlurScaleX = 0.5f;
	// postCmp->BloomBlurScaleY = 100.0f;
	postCmp->AbberationScale = 0.5f;
	postCmp->GrainScale = 0.5f;
	postCmp->MotionBlurScale = 0.5f;
	postCmp->VignetteScale = 2.0f;
	// postCmp->FogColor = Color(0.5f, 0.4f, 0.2f) * 0.1f;
	// postCmp->FogDensity = 0.66f;
	// postCmp->Temperature = 3500.0f;
	postCmp->Tint = Color(2.0f, 1.8f, 0.5f); // Industrial Hall Grade
	// gameMgrCmp->PostCmp->DOFShow = 1.0f;
}

void GameManagerSystem::PostProcessNone(PostprocessSettingsComponent* postCmp)
{
	postCmp->Exposure = 2.0f;
	postCmp->DOFSize = 0.0f;
	postCmp->DOFPoint = 1000.0f;
	postCmp->DOFRange = 2000.0f;
	postCmp->BloomScale = 0.0f;
	// postCmp->BloomBlurScaleX = 1.0f;
	// postCmp->BloomBlurScaleY = 1.0f;
	postCmp->AbberationScale = 0.0f;
	postCmp->GrainScale = 0.0f;
	postCmp->MotionBlurScale = 0.0f;
	postCmp->VignetteScale = 0.0f;
	// postCmp->FogColor = Color::WHITE;
	// postCmp->FogDensity = 0.0f;
	// postCmp->Temperature = 6500.0f;
	// postCmp->DOFShow = 1.0f;
}

void GameManagerSystem::CreateTextUI(Scene* scene)
{
	Entity* text = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<ScreenSpaceTextComponent>(
		scene,
		text,
		Vector2i(50, 50),
		"Fonts/Raleway/Raleway-Regular.ttf",
		eResourceSource::ENGINE,
		40.0f,
		"powered by PolyEngine"
	);
}

Entity* GameManagerSystem::CreateModel(Scene* scene, String path)
{
	Entity* model = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<DebugDrawableComponent>(scene, model, eDebugDrawPreset::STATIC);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, model, path, eResourceSource::GAME);
	return model;
}

void GameManagerSystem::CreateCamera(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 35_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);

	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-550.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));
	gameMgrCmp->Camera = camera;

	Entity* keyDirLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<DirectionalLightComponent>(scene, keyDirLight, Color(1.0f, 1.0f, 1.0f), 15.0f);
	// keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_Y, -45_deg) * Quaternion(Vector::UNIT_X, -65_deg));
	keyDirLight->GetTransform().SetGlobalRotation(Quaternion(Vector::UNIT_X, 60_deg) * Quaternion(Vector::UNIT_Y, -10_deg));
	gameMgrCmp->GameEntities.PushBack(keyDirLight);
}

void GameManagerSystem::CreatePBRShpereGrid(Scene* scene, Vector pos, Color albedo)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	for (int z = 0; z < 5; ++z)
	{
		for (int y = 0; y < 5; ++y)
		{
			Entity* sphere = DeferredTaskSystem::SpawnEntityImmediate(scene);
			EntityTransform& sphereTrans = sphere->GetTransform();
			sphereTrans.SetGlobalTranslation(pos + Vector(50.0f * y, 0.0f, 50.0f * z) - (Vector(50.0f * 5.0f, -100.0f, 50.0f * 5.0f) * 0.5f));
			sphereTrans.SetLocalScale(Vector(1.0f, 1.0f, 1.0f) * 20.0f);
			sphereTrans.SetLocalRotation(Quaternion(Vector::UNIT_X, 90.0_deg));

			MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sphere, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);

			size_t materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
			for (size_t i = 0; i < materialsNum; ++i)
			{
				float Roughness	= ((0.01f + z) / 5.0f);
				float Metallic	= ((0.01f + y) / 5.0f);
				meshCmp->SetMaterial(i, Material(
					Color(0.0f, 0.0f, 0.0f, 1.0f),
					albedo,
					Roughness,
					Metallic,
					0.5f
				));
				meshCmp->SetBlendingMode(eBlendingMode::OPAUQE);
			}

			gameMgrCmp->GameEntities.PushBack(sphere);
		}
	}
}

void GameManagerSystem::CreateRandomCubes(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	for (int i = 0; i < 50; ++i)
	{
		Entity* cube = DeferredTaskSystem::SpawnEntityImmediate(scene);
		EntityTransform& cubeTrans = cube->GetTransform();
		MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, cube, "Models/Primitives/Cube.obj", eResourceSource::GAME);
		meshCmp->SetMaterial(0, Material(Color::BLACK, Color::WHITE * 0.5f, 0.5f, 0.5f, 0.5f));
		
		Vector rndPos = RandomVectorRange(-1.0f, 1.0f);
		Vector rndRot = RandomVectorRange(-1.0f, 1.0f);
		Vector rndScale = RandomVector();
		Vector position = Vector::UNIT_Y * 300.0f + Vector(2000.0f * rndPos.X, 50.0f * rndPos.Y, 400.0f * rndPos.Z);

		cubeTrans.SetLocalScale(Vector::ONE * 10.0f + rndScale * 200.0f);
		cubeTrans.SetGlobalTranslation(position);
		cubeTrans.SetLocalRotation(
			  Quaternion(Vector::UNIT_X, 180.0_deg * rndRot.X)
			* Quaternion(Vector::UNIT_Y, 180.0_deg * rndRot.Y)
			* Quaternion(Vector::UNIT_Z, 180.0_deg * rndRot.Z)
		);

		gameMgrCmp->GameEntities.PushBack(cube);
	}
}

void GameManagerSystem::CreateTranslucent(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Material material(
		Color(0.0f, 0.0f, 0.0f, 1.0f),
		Color(1.0f, 1.0f, 1.0f, 0.5f),
		0.1f,
		1.0f,
		0.5f
	);

	for (int i = 0; i < 8; ++i)
	{
		Entity* translucent = DeferredTaskSystem::SpawnEntityImmediate(scene);
		EntityTransform& translucentTrans = translucent->GetTransform();
		Vector randomOffset = RandomVectorRange(-1.0f, 1.0f) * 200.0f;
		translucentTrans.SetGlobalTranslation(Vector(0.0f, 200.0f, 0.0f) + randomOffset);
		translucentTrans.SetLocalScale(Vector(100.0f, 100.0f, 100.0f));
		MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, translucent, "Models/Primitives/Sphere_HighPoly.obj", eResourceSource::GAME);
		size_t materialsNum = meshCmp->GetMesh()->GetSubMeshes().GetSize();
		for (size_t i = 0; i < materialsNum; ++i)
		{
			meshCmp->SetMaterial(i, material);
			meshCmp->SetBlendingMode(eBlendingMode::TRANSLUCENT);
		}

		gameMgrCmp->GameEntities.PushBack(translucent);
	}
}

void GameManagerSystem::CreateSponza(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 35_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 100.0f, 0.003f, 10.0f);
	gameMgrCmp->PostCmp = DeferredTaskSystem::AddComponentImmediate<PostprocessSettingsComponent>(scene, camera);
	// PostProcessFancy(gameMgrCmp->PostCmp);
	PostProcessNone(gameMgrCmp->PostCmp);
	
	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(-550.0f, 180.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, -90.0_deg) * Quaternion(Vector::UNIT_X, -10.0_deg));
	// cameraTrans.SetGlobalRotation(Quaternion(Matrix(cameraTrans.GetGlobalTranslation(), Vector(0.0f, 0.0f, 0.0f))));
	scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));
	gameMgrCmp->Camera = camera;
	
	DeferredTaskSystem::AddWorldComponentImmediate<SkyboxWorldComponent>(scene, "HDR/HDR.hdr", eResourceSource::GAME);

	Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sponza, "Models/Sponza/sponza.obj", eResourceSource::GAME);

	gameMgrCmp->GameEntities.PushBack(sponza);

	CreatePointLights(scene, 100);
}

void GameManagerSystem::CreateSponzaSample(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* camera = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<CameraComponent>(scene, camera, 40_deg, 1.0f, 5000.f);
	DeferredTaskSystem::AddComponentImmediate<FreeFloatMovementComponent>(scene, camera, 10.0f, 0.003f);
	EntityTransform& cameraTrans = camera->GetTransform();
	cameraTrans.SetGlobalTranslation(Vector(300.0f, 0.0f, 0.0f));
	cameraTrans.SetGlobalRotation(Quaternion(Vector::UNIT_Y, 90.0_deg));

	scene->GetWorldComponent<ViewportWorldComponent>()->SetCamera(0, scene->GetComponent<CameraComponent>(camera));

	Entity* sponza = DeferredTaskSystem::SpawnEntityImmediate(scene);
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, sponza, "Models/Sponza/sponza_lion.obj", eResourceSource::GAME);
	gameMgrCmp->GameEntities.PushBack(sponza);

	Vector lightPosition = Vector(50.0f, 0.0f, 50.0f);
	Entity* lightEntity = CreatePointLight(scene, lightPosition, 100.0f);
	gameMgrCmp->LightsStartPositions.PushBack(lightPosition);
	gameMgrCmp->PointLightEntities.PushBack(lightEntity);
}

void GameManagerSystem::DrawFrustumPoints(Scene* scene, Dynarray<Vector> &cornersInWorld, Color color)
{
	for (size_t i = 0; i < 4; ++i)
		DebugDrawSystem::DrawLine(scene, cornersInWorld[i], cornersInWorld[i + 4], color);

	for (size_t i = 0; i < 2; ++i)
	{
		DebugDrawSystem::DrawLine(scene, cornersInWorld[0 + i * 4], cornersInWorld[1 + i * 4], color);
		DebugDrawSystem::DrawLine(scene, cornersInWorld[2 + i * 4], cornersInWorld[3 + i * 4], color);
		DebugDrawSystem::DrawLine(scene, cornersInWorld[0 + i * 4], cornersInWorld[2 + i * 4], color);
		DebugDrawSystem::DrawLine(scene, cornersInWorld[1 + i * 4], cornersInWorld[3 + i * 4], color);
	}
}

void GameManagerSystem::Update(Scene* scene)
{
	UpdateLights(scene);

	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	DirectionalLightComponent* dirLight = gameMgrCmp->KeyDirLight.Get()->GetComponent<DirectionalLightComponent>();

	float arrowLength = 50.0f;
	Vector axesPivot = Vector::UNIT_Y * 500.0f;
	DebugDrawSystem::DrawLine(scene, axesPivot, axesPivot + MovementSystem::GetGlobalForward(dirLight->GetTransform()) * arrowLength,  Color(1.0f, 1.0f, 0.0f));
	DebugDrawSystem::DrawLine(scene, axesPivot, axesPivot + Vector::UNIT_X * arrowLength, Color::RED);
	DebugDrawSystem::DrawLine(scene, axesPivot, axesPivot + Vector::UNIT_Y * arrowLength, Color::GREEN);
	DebugDrawSystem::DrawLine(scene, axesPivot, axesPivot + Vector::UNIT_Z * arrowLength, Color::BLUE);

	// UpdateCameraAspect(scene);

	// UpdateSkybox(scene);

	// UpdateModel(scene);
	

	// DEBUG: Recreate camera matrices for test
	Vector frustumPosition = Vector::UNIT_Z * 100.0f + Vector::UNIT_X * 2000.0f * Cos(10_deg * time);
	Quaternion frustumRotationQuat = Quaternion(Vector::UNIT_X, 120_deg * Sin(5_deg * time))
		* Quaternion(Vector::UNIT_Y, 120_deg * Sin(-10_deg * time));

	Matrix frustumTranslationMat;
	frustumTranslationMat.SetTranslation(frustumPosition);

	Matrix clipFromView;
	clipFromView.SetPerspective(45_deg, 2.30f, 1.0f, 500.0f);
	Matrix viewFromWorld = frustumRotationQuat.ToRotationMatrix() * frustumTranslationMat;
	Matrix clipFromWorld = clipFromView * viewFromWorld;

	// Transform frustum corners to DirLightSpace
	Dynarray<Vector> cornersInNDC {
		Vector(-1.0f,  1.0f, -1.0f), // back  left	top
		Vector( 1.0f,  1.0f, -1.0f), // back  right top
		Vector(-1.0f, -1.0f, -1.0f), // back  left  bot
		Vector( 1.0f, -1.0f, -1.0f), // back  right bot
		Vector(-1.0f,  1.0f,  1.0f), // front left	top
		Vector( 1.0f,  1.0f,  1.0f), // front right top
		Vector(-1.0f, -1.0f,  1.0f), // front left  bot
		Vector( 1.0f, -1.0f,  1.0f)	 // front right bot
	};

	// Transform frustum corners from NDC to World
	// could be done in one iteration but we need to do perspective division by W
	Matrix worldFromClip = clipFromWorld.GetInversed();
	Dynarray<Vector> cornersInWS;
	for (Vector posInClip : cornersInNDC)
	{
		Vector posInWS = worldFromClip * posInClip;
		posInWS.X /= posInWS.W;
		posInWS.Y /= posInWS.W;
		posInWS.Z /= posInWS.W;
		cornersInWS.PushBack(posInWS);
	}

	DrawFrustumPoints(scene, cornersInWS, Color::RED);

	// based on https://mynameismjp.wordpress.com/2013/09/10/shadow-maps/
	// Stabilize shadow map: calculate sphere bounds around frustum to minimize AABB changes on frustum rotation 
	// Calculate the centroid of the view frustum slice
	Vector frustumCenterInWS = Vector::ZERO;
	for (Vector posInWS : cornersInWS)
	{
		frustumCenterInWS = frustumCenterInWS + posInWS;
	}
	frustumCenterInWS *= 1.0f / 8.0f;

	float maxRadius = 0.0f;
	for (Vector posInWS : cornersInWS)
	{
		float radius = (cornersInWS[0] - posInWS).Length();
		maxRadius = std::max(maxRadius, radius);
	}
	maxRadius = std::ceilf(maxRadius * 16.0f) / 16.0f;
	DebugDrawSystem::DrawSphere(scene, frustumCenterInWS, maxRadius);

	Vector frustumMinInWS = frustumCenterInWS - Vector::ONE * maxRadius;
	Vector frustumMaxInWS = frustumCenterInWS + Vector::ONE * maxRadius;
	AABox frustumAABBInWS(frustumMinInWS, frustumMaxInWS - frustumMinInWS);
	DebugDrawSystem::DrawBox(scene, frustumMinInWS, frustumMaxInWS, Color::BLUE);

	// Transform frustum corners from World to DirLight
	// Matrix worldFromDirLight = dirLight->GetTransform().GetWorldFromModel();
	// Matrix dirLightFromWorld = worldFromDirLight.GetInversed();

	// AABox frustumAABBinLS = frustumAABBInWS.GetTransformed(dirLightFromWorld);
	// FindShadowCasters(scene, dirLightFromWorld, worldFromDirLight, frustumAABBinLS);

	// convert height of frustumAABBinLS extended by shadow casters to world space
	// Vector minTmp = worldFromDirLight * frustumAABBinLS.GetMin();
	// Vector maxTmp = worldFromDirLight * frustumAABBinLS.GetMax();
	// DebugDrawSystem::DrawBox(scene, minTmp, maxTmp, Color(0.0f, 1.0f, 1.0f));

	// extend world space frustum AABB by height from frustumAABBinLS
	// Vector center = frustumAABBInWS.GetCenter();
	// frustumAABBInWS.Expand(Vector(center.X, center.Y, std::min(frustumAABBInWS.GetMin().Z, minTmp.Z)));
	// frustumAABBInWS.Expand(Vector(center.X, center.Y, std::max(frustumAABBInWS.GetMax().Z, maxTmp.Z)));
	// DebugDrawSystem::DrawBox(scene, frustumAABBInWS, Color(1.0f, 0.0f, 1.0f));
	dirLight->DebugShadowAABBInWS = frustumAABBInWS;
}

void GameManagerSystem::FindShadowCasters(Scene* scene, const Matrix &dirLightFromWorld, const Matrix &worldFromDirLight, AABox &frustumAABBInLS)
{
	const float maxFloat = std::numeric_limits<float>::max();

	Dynarray<MeshRenderingComponent*> meshCmps;
	for (auto& [meshCmp] : scene->IterateComponents<MeshRenderingComponent>())
	{
		meshCmp->IsShadowCaster = false;
		meshCmps.PushBack(meshCmp);
	}
	// transform meshes AABB to DirLightSpace
	Dynarray<std::tuple<AABox, MeshRenderingComponent*>> meshBoxes;
	for (const auto& meshCmp : meshCmps)
	{
		const Matrix& dirLightFromModel = dirLightFromWorld * meshCmp->GetTransform().GetWorldFromModel();
		Optional<AABox> boxWSOptional = meshCmp->GetBoundingBox(eEntityBoundingChannel::RENDERING);
		if (boxWSOptional.HasValue())
		{
			AABox boxWS = boxWSOptional.Value();
			AABox boxLS = boxWS.GetTransformed(dirLightFromModel);
			meshBoxes.PushBack(std::tuple(boxLS, meshCmp));
			// DebugDrawSystem::DrawBox(scene, boxLS.GetMin(), boxLS.GetMax(), worldFromDirLight, Color::WHITE);
		}
	}

	// find min Z for near clipping plane and max Z for far clipping plane
	float minZ = maxFloat;
	float maxZ = -maxFloat;
	for (const auto& [boxLS, mesh] : meshBoxes)
	{
		if (!(boxLS.IntersectsXY(frustumAABBInLS))) // extend dir light AAbox only in Z based on objects in rect defined on dir light AABob XY plane
			continue;

		minZ = std::min(minZ, boxLS.GetMin().Z);
		maxZ = std::max(maxZ, boxLS.GetMax().Z);
	}

	Vector center = frustumAABBInLS.GetCenter(); // X and Y should be neutral so AABB expanded only on Z axis
	frustumAABBInLS.Expand(Vector(center.X, center.Y, minZ))
					.Expand(Vector(center.X, center.Y, maxZ));

	DebugDrawSystem::DrawBox(scene, frustumAABBInLS.GetMin(), frustumAABBInLS.GetMax(), worldFromDirLight, Color(1.0f, 1.0f, 0.0f));

	// find all meshes that are inside extended DirLights AABB box
	int shadowCastersCounter = 0;
	for (auto&[box, meshCmp] : meshBoxes)
	{
		bool IsOverlap = false;
		std::array<Vector, 8> meshAABoxVerts = box.GetVertices();
		for (auto& v : meshAABoxVerts)
			IsOverlap |= frustumAABBInLS.Contains(v);

		std::array<Vector, 8> lightAABoxVerts = frustumAABBInLS.GetVertices();
		for (auto& v : lightAABoxVerts)
			IsOverlap |= box.Contains(v);

		if (IsOverlap || box.Intersects(frustumAABBInLS))
		{
			meshCmp->IsShadowCaster = true;

			DebugDrawSystem::DrawBox(scene, box.GetMin(), box.GetMax(), worldFromDirLight, Color::GREEN);
			shadowCastersCounter++;
		}
	}

	gConsole.LogInfo("GameManagerSystem::FillDirLightQueue casters: {}/{}", shadowCastersCounter, meshCmps.GetSize());
}

void GameManagerSystem::UpdatePostProcess(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	time *= 0.1f;
	float exposure = 0.25f * floor(10.0f*4.0f * (time - floor(time)));
	gameMgrCmp->PostCmp->Exposure = exposure;
	gConsole.LogInfo("GameManagerSystem::Update exposure: {}", gameMgrCmp->PostCmp->Exposure);
}

void GameManagerSystem::Deinit(Scene* scene)
{
	gConsole.LogInfo("GameManagerSystem::Deinit");
}

void GameManagerSystem::CreatePointLights(Scene* scene, int quota)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	gameMgrCmp->LightsStartPositions.Reserve(quota);

	for (int i = 0; i < quota; ++i)
	{
		Vector position = Vector(RandomRange(-1.0f, 1.0f)*1000.0f, RandomRange(0.0f, 800.0f), RandomRange(-1.0f, 1.0f)*500.0f);
		float rangeRnd = pow(RandomRange(0.0f, 1.0f), 8.0f);
		float range = 100.0f + rangeRnd * 2000.0f;
		Entity* lightEntity = CreatePointLight(scene, position, range);

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

void GameManagerSystem::UpdateLights(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	if (gameMgrCmp->PointLight)
	{
		float r = 200.0f;
		Angle a = 0.5_rad * time;
		float s = Sin(a);
		float c = Cos(a);
		gameMgrCmp->PointLight->GetTransform().SetGlobalTranslation(Vector(c, 0.0f, s) * r + Vector::UNIT_Y * 200.0f);
	}

	if (gameMgrCmp->KeyDirLight)
	{
		float anim = Sin(0.5_rad * time) * 0.5f + 0.5f;
		// float smooth = SmoothStep(0.1f, 0.9f, anim);
		gameMgrCmp->KeyDirLight->GetTransform().SetGlobalRotation(
			  Quaternion(Vector::UNIT_X, Lerp(0.0_deg, -180.0_deg, anim))
		);
	}

	for (size_t i = 0; i < gameMgrCmp->LightsStartPositions.GetSize(); ++i)
	{
		Entity* pointLight = gameMgrCmp->PointLightEntities[i];
		Vector startPosition = gameMgrCmp->LightsStartPositions[i];
		Vector offset = Vector::UNIT_Y * Sin(2.0_rad * time + 1.0_rad * startPosition.X) * 100.0f;
		EntityTransform& pointLightTrans = pointLight->GetTransform();
		pointLightTrans.SetGlobalTranslation(startPosition + offset);
	}
}

void GameManagerSystem::UpdateModel(Scene* scene)
{
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	if (gameMgrCmp->Model)
	{
		EntityTransform& modelTrans = gameMgrCmp->Model->GetTransform();
		modelTrans.SetLocalRotation(Quaternion(Vector::UNIT_Y, 10.0_deg * time));
	}
}

void GameManagerSystem::UpdateCameraAspect(Poly::Scene * scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();
	CameraComponent* cameraCmp = gameMgrCmp->Camera->GetComponent<CameraComponent>();
	float time = (float)(scene->GetWorldComponent<TimeWorldComponent>()->GetGameplayTime());
	float anim = SmoothStep(-0.8f, 0.8f, Sin(5.0_rad * time));
	float aspect = Lerp(2.5f, 0.5f, anim);
	cameraCmp->SetAspect(aspect);
	gConsole.LogInfo("GameManagerSystem::Update aspect: {}", aspect);
}

Entity* GameManagerSystem::CreatePointLight(Scene* scene, Vector& position, float Range)
{
	Color lightColor = Color(RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f), RandomRange(0.0f, 1.0f));

	Entity* pointLight = DeferredTaskSystem::SpawnEntityImmediate(scene);
	PointLightComponent* pointLightCmp = DeferredTaskSystem::AddComponentImmediate<PointLightComponent>(scene, pointLight, lightColor, 100.0f * 1000.0f, Range);
	MeshRenderingComponent* meshCmp = DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, pointLight, "Models/Primitives/Sphere_LowPoly.obj", eResourceSource::GAME);
	meshCmp->SetMaterial(0, Material(Color(0.0f, 0.0f, 0.0f, 0.0f), lightColor, 0.0f, 1.0f, 0.5f));
	EntityTransform& pointLightTrans = pointLight->GetTransform();
	pointLightTrans.SetGlobalScale(Vector::ONE * 5.0f);
	pointLightTrans.SetGlobalTranslation(position);

	return pointLight;
}

void GameManagerSystem::SpawnShaderball(Scene* scene)
{
	GameManagerWorldComponent* gameMgrCmp = scene->GetWorldComponent<GameManagerWorldComponent>();

	Entity* shaderball = DeferredTaskSystem::SpawnEntityImmediate(scene);
	EntityTransform& shaderballTrans = shaderball->GetTransform();
	shaderballTrans.SetLocalTranslation(Vector(0.0f, 5.0f, 0.0f));
	DeferredTaskSystem::AddComponentImmediate<MeshRenderingComponent>(scene, shaderball, "Models/shaderball/PolyEngine_shaderball.fbx", eResourceSource::GAME);
	MeshRenderingComponent* ballMesh = scene->GetComponent<MeshRenderingComponent>(shaderball);
	ballMesh->SetMaterial(0, Material(Color(1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 0.0f), 1.0f, 1.0f, 0.5f));
	ballMesh->SetMaterial(1, Material(Color(1.0f, 1.0f, 1.0f), Color(0.4f, 0.4f, 0.4f), 1.0f, 1.0f, 0.5f));
	shaderballTrans.SetLocalScale(0.1f);
	gameMgrCmp->GameEntities.PushBack(shaderball);
}