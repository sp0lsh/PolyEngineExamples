#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void Update(World* world);
	void UpdateCamera(World* world);
	void UpdateParticles(World* world);
	void Deinit(World* world);

	void SpawnShip(World* world);

	void SpawnShaderball(World* world);
	void SpawnSponzaScene(World* world);

	
	void SpawnSpritesheerExamples(World* world);
	void SpawnSpritesheet11(World* world, Vector pos);
	void SpawnSpritesheet22(World* world, Vector pos);
	void SpawnSpritesheet44(World* world, Vector pos);
	void SpawnSpritesheet42(World* world, Vector pos);
	void SpawnSpritesheet41(World* world, Vector pos);
	void SpawnSpritesheet44Random(World* world, Vector pos);
	void SpawnSpritesheetGandalf(World* world, Vector pos);


	void SpawnParticleExamples(World* world);
	ParticleComponent* SpawnEmitterDefault(World* world, Vector pos);
	ParticleComponent* SpawnEmitterHeart(World* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact(World* world, Vector pos);
	ParticleComponent* SpawnEmitterHeartImpact2(World* world, Vector pos);
	ParticleComponent* SpawnEmitterWorldSpace(World* world, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(World* world, Vector pos);

	ParticleComponent* SpawnSmokeEmitterInWS(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnSmokeBurstEmitterInWS(World* world, Entity* parent, Vector offset);


	void AddPointLights(World* world, int Quata);
	void CreatePointLight(World* world, float Range);
	void CreateSpotLight(World * world, float Range);
}
