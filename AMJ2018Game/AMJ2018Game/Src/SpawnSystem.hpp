#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace SpawnSystem
{
	void Update(World* world);
	void Init(World* world);

	void SpawnEnemies(World* world);

}