#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void CreateSponza(World* world);
	void CreateSponzaSample(World* world);
	void Update(World* world);
	void Deinit(World* world);

	void CreatePointLights(World* world, int quota);
	void UpdateLights(World* world);
	Entity* CreatePointLight(World* world, Vector& position, float Range);

	float Random();
	float Random(float min, float max);
	void CreateSpotLight(World * world, float Range);
}
