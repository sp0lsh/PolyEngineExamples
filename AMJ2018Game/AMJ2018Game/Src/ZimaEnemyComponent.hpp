#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

enum class EnemyType
{
	SinusRandom,
	Shooting,
	_COUNT
};

class GAME_DLLEXPORT ZimaEnemyComponent : public ComponentBase
{
public:
	ZimaEnemyComponent(EnemyType type, float speed, float health) :
		Type(type), Speed(speed), MaxHealth(health), CurrentHealth(health) {};

	float Speed = 200.f;
	float MaxHealth = 100.f;
	float DamageOnHit = 15.f;

	float CurrentHealth;
	float DamageToBeDealt = 0.f;
	bool bDead = false;

	EnemyType Type;
};
