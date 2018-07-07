#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class GAME_DLLEXPORT ZimaGunComponent : public ComponentBase
{
public:
	ZimaGunComponent(Vector localTranslation, Quaternion localRotation) :LocalTranslation(localTranslation), LocalRotation(localRotation) {};
	float BulletSpawnInterval = 0.2f;

	float TimeSinceLastBullet = 1000.f;
	bool bSpawnBullet = false;

	Vector LocalTranslation;
	Quaternion LocalRotation;
};
