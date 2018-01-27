#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void SpawnShaderball(World* world);
	void SpawnSponzaScene(World* world);
	void Update(World* world);
	void Deinit(World* world);

	ParticleComponent* SpawnEmitter0(World* world);
	ParticleComponent* SpawnEmitter1(World* world);
	ParticleComponent* SpawnEmitter2(World* world);
	ParticleComponent* SpawnEmitter3(World* world);
	void SpawnSpritesheet(World* world);

	void AddPointLights(World* world, int Quata);
	void CreatePointLight(World* world, float Range);
	void CreateSpotLight(World * world, float Range);

	float Random();
	float Random(float min, float max);
	Vector RandomVector(float min, float max);
}
