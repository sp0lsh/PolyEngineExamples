#pragma once

#include <ECS/ComponentBase.hpp>
#include <ZimaEnemyComponent.hpp>
using namespace Poly;


struct SpawnWave : public BaseObject<>
{
	SpawnWave(Dynarray<Vector> transforms, Dynarray<EnemyType> types) :
		enemyTransforms(transforms), Types(types) {};

	Dynarray<Vector> enemyTransforms;
	Dynarray<EnemyType> Types;
	float WaitAfterWave = 2.f;

};

class GAME_DLLEXPORT SpawnWorldComponent : public ComponentBase
{
public:
	Dynarray<Vector> enemyTransforms
	{
		Vector(100.f, 0.f, 200.f),
		Vector(100.f, 0.f, 100.f),
		Vector(100.f, 0.f, -100.f),
		Vector(100.f, 0.f, -200.f)
	};

	Dynarray<EnemyType> enemyTypes
	{
		EnemyType::SinusRandom,
		EnemyType::Shooting,
		EnemyType::Shooting,
		EnemyType::SinusRandom
	};


	Dynarray<SpawnWave> Waves
	{
		SpawnWave(enemyTransforms, enemyTypes)
	};
	

		float WaveInterval = 2.f;
	float TimeSinceLastWave = 0.f;

};