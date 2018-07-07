#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class GAME_DLLEXPORT ZimaEnemyComponent : public ComponentBase
{
public:
	ZimaEnemyComponent(float speed, float health) :
		Speed(speed), MaxHealth(health), CurrentHealth(health) {};

	float Speed = 200.f;
	float MaxHealth = 100.f;
	float DamageOnHit = 15.f;

	float CurrentHealth;
	float DamageToBeDealt = 0.f;
	bool bDead = false;
};
