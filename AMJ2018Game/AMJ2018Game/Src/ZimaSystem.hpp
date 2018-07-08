#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Rendering/Particles/ParticleComponent.hpp>

using namespace Poly;

namespace ZimaSystem
{
	void Init(World* world);


	Entity* CreateActor(World* world, String path);

	void CreateCamera(World* world);

	bool IsColliding(Entity* EntityA, float RadiusA, Entity* EntityB, float RadiusB);
	void ReadQuote(World* world, String path);
	
	ParticleComponent* SpawnEmitterLocalSpace(World* world, Vector pos);

	ParticleComponent* SpawnEmitterHeartImpact(World* world, Vector pos);
	void UpdateParticles(World* world);

	void Update(World* world);
	void Deinit(World* world);
}
