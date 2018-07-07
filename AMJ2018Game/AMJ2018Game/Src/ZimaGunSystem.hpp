#pragma once

#include <ECS/World.hpp>

using namespace Poly;

namespace ZimaGunSystem
{
	void Update(World* world);
	void SpawnBullet(World* world, const Vector& translation, const Quaternion& quaternion);
}
