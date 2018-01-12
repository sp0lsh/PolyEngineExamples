#pragma once

#include <World.hpp>

namespace BT
{
	namespace GameManagerSystem
	{
		void Update(Poly::World* world);

		void InitializeDemoWorld(Poly::World* world);

		void SpawnBullet(Poly::World* world);
	}
}