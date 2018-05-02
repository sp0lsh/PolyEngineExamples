#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void Update(World* world);
	void Deinit(World* world);

	void CreateTestLights(World* world, int quota);
	void UpdateTestLights(World* world);
	Entity* CreatePointLight(World* world, Vector& position, float Range);

	float Random();
	float Random(float min, float max);
	void CreateSpotLight(World * world, float Range);
}
