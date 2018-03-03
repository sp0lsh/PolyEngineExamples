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
	void UpdateShip(World* world);
	void UpdateGameplay(World* world);
	void UpdateParticles(World* world);
	void Deinit(World* world);
	
	void RestartGame(World* world);
	
	void SpawnShip(World* world);
	void SpawnBomb(World* world, Vector pos);
	bool CollideWithBomb(Entity* ShipCollision, Entity* Bomb);

	void SpawnParticleExamples(World* world);

	ParticleComponent* SpawnEmitterWorldSpace(World* world, Vector pos);
	ParticleComponent* SpawnEmitterLocalSpace(World* world, Vector pos);

	ParticleComponent* SpawnSmokeEmitterInWS(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnSmokeBurstEmitterInWS(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnEngineEmitterInWS(World* world, Entity* parent, Vector offset);
	ParticleComponent* SpawnEngineBurstEmitterInWS(World* world, Entity* parent, Vector offset);
}
