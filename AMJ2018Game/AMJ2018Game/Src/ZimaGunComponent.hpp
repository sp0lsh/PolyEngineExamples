#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class GAME_DLLEXPORT ZimaGunComponent : public ComponentBase
{
public:
	ZimaGunComponent(Vector localTranslation, Dynarray<Quaternion>  localRotations, float bulletSpawnInterval = 0.2f) :LocalTranslation(localTranslation), LocalRotations(localRotations), BulletSpawnInterval(bulletSpawnInterval) {};
	float BulletSpawnInterval = 0.2f;

	float TimeSinceLastBullet = 0.f;
	bool bSpawnBullet = false;

	Vector LocalTranslation;
	Dynarray<Quaternion> LocalRotations;
};
