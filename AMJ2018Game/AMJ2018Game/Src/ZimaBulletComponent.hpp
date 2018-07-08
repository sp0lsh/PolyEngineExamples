#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class GAME_DLLEXPORT ZimaBulletComponent : public ComponentBase
{
public:
	float Speed = 300.f;
	float LifeTime = 0.f;
	float MaxLifetime = 2.f;
	float Damage = 120.f;

	bool bDead = false;
	SafePtr<Entity> Instigator;
};
