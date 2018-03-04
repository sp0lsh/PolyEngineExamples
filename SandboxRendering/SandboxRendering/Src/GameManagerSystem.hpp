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
	void UpdatePlayer(World* world);
	Entity* SpawnTorpedo(World* world, Vector &TorpedoSpawnPos, Quaternion &TorpedoSpawnRot);
	void UpdateEnemies(World* world);
	void UpdateGameplay(World* world);
	void UpdateParticles(World* world);
	void UpdatePostEffect(World* world);
	void Deinit(World* world);
	
	void RestartGame(World* world);
	
	void SpawnShip(World* world);
	void SpawnEnemyShip(World* world);
	void SpawnCamera(World* world);
	void SpawnBomb(World* world, Vector pos);
	bool IsColliding(Entity* EntityA, float RadiusA, Entity* EntityB, float RadiusB);

	void SpawnParticleExamples(World* world);

	ParticleComponent* SpawnEmitterWorldSpace(World* world, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(World* world, Vector pos);

	ParticleComponent* SpawnSmokeEmitterInWS(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnSmokeBurstEmitterInWS(World* world, Entity* parent, Vector offset);
	
	ParticleComponent* SpawnExplosionEmitterInWS(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnExplosionEmitterInWS2(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnExplosionEmitterInWS3(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnExplosionEmitterInWS4(World* world, Entity* parent, Vector offset);
}
