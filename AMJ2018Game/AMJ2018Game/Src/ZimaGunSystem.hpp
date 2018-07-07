#pragma once

#include <ECS/World.hpp>
#include "ZimaGunComponent.hpp"

using namespace Poly;

namespace ZimaGunSystem
{
	void Update(World* world);
	void SpawnBullet(World* world,  ZimaGunComponent* gunComp, const Vector& translation, const Quaternion& quaternion);
}
