#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void Init(Scene* world);

	Entity* CreateModel(Scene* world, String path);
	
	void CreateTextUI(Scene* world);
	void CreateCamera(Scene* world);
	void CreatePBRShpereGrid(Scene* world, Vector pos, Color albedo);
	void CreateTranslucent(Scene* world);
	void CreateSponza(Scene* world);
	void CreateSponzaSample(Scene* world);
	
	void CreatePointLights(Scene* world, int quota);
	Entity* CreatePointLight(Scene* world, Vector& position, float Range);
	void CreateSpotLight(Scene* world, float Range);
	void CreateParticles(Scene* world);	
	void SpawnShaderball(Scene* world);

	void Update(Scene* world);
	void UpdateLights(Scene* world);
	void UpdateModel(Scene* world);
	void UpdatePostProcess(Scene* world);

	ParticleComponent* SpawnEmitterAmbient(Scene* scene, Vector pos);
	
	void Deinit(Scene* world);
}
