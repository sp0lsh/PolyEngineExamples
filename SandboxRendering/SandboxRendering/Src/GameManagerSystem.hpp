#pragma once

#include <ECS/World.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>

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

	void CreateSpotLight(World * world, float Range);
}
