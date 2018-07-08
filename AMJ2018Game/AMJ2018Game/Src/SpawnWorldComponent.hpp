#pragma once

#include <ECS/ComponentBase.hpp>
#include <ZimaEnemyComponent.hpp>
using namespace Poly;


struct SpawnWave : public BaseObject<>
{
	SpawnWave(Dynarray<Vector> transforms, Dynarray<EnemyType> types, float waitAfterWave = 2.f) :
		enemyTransforms(transforms), Types(types), WaitAfterWave(waitAfterWave) {};

	SpawnWave(const SpawnWave& wave, float waitAfterWave) :
		enemyTransforms(wave.enemyTransforms), Types(wave.Types), Offset(wave.Offset), WaitAfterWave(waitAfterWave) {};
	SpawnWave(const SpawnWave& wave, Vector offset) :
		enemyTransforms(wave.enemyTransforms), Types(wave.Types), WaitAfterWave(wave.WaitAfterWave), Offset(offset) {};

	Dynarray<Vector> enemyTransforms;
	Dynarray<EnemyType> Types;
	float WaitAfterWave = 2.f;
	Vector Offset;
};

class GAME_DLLEXPORT SpawnWorldComponent : public ComponentBase
{
public:
	Dynarray<Vector> enemyTransforms
	{
		Vector(140.f, 0.f, 200.f),
		Vector(140.f, 0.f, 100.f),
		Vector(140.f, 0.f, -100.f),
		Vector(140.f, 0.f, -200.f)
	};

	Dynarray<Vector> enemyTransforms2
	{
		Vector(140.f, 0.f, 250.f),
		Vector(140.f, 0.f, 200.f),
		Vector(140.f, 0.f, 150.f),
		Vector(140.f, 0.f, 100.f),
		Vector(140.f, 0.f, 50.f),
		Vector(140.f, 0.f, 00.f),
		Vector(140.f, 0.f, -50.f),
		Vector(140.f, 0.f, -100.f),
		Vector(140.f, 0.f, -150.f),
		Vector(140.f, 0.f, -200.f),
		Vector(140.f, 0.f, -250.f)
	};

	SpawnWave Simple = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::SinusRandom,
			EnemyType::SinusRandom,
			EnemyType::SinusRandom,
			EnemyType::SinusRandom
	}, 3.f);
	SpawnWave WaitSimple = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::SinusRandom,
			EnemyType::SinusRandom,
			EnemyType::SinusRandom,
			EnemyType::SinusRandom
	}, 5.f);


	SpawnWave SinusShooting = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::None,
			EnemyType::SinusShooting,
			EnemyType::SinusShooting,
			EnemyType::None
	}, 3.f);

	SpawnWave Shooting2 = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::SinusShooting,
			EnemyType::SinusShooting,
			EnemyType::SinusShooting,
			EnemyType::SinusShooting
	}, 3.f);

	SpawnWave Shooting3 = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::Wachlarz,
			EnemyType::SinusShooting,
			EnemyType::SinusShooting,
			EnemyType::Wachlarz
	}, 3.f);

	SpawnWave Shooting35 = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::SinusWachlarz,
			EnemyType::None,
			EnemyType::None,
			EnemyType::SinusWachlarz,
	}, 3.f);

	SpawnWave Shooting4 = SpawnWave(enemyTransforms,
		Dynarray<EnemyType>
	{
		EnemyType::Wachlarz,
			EnemyType::Wachlarz,
			EnemyType::Wachlarz,
			EnemyType::Wachlarz
	}, 3.f);

	SpawnWave Armaggedon = SpawnWave(enemyTransforms2,
		Dynarray<EnemyType>
	{
		EnemyType::Shooting,
			EnemyType::Wachlarz,
			EnemyType::Shooting,
			EnemyType::Wachlarz,
			EnemyType::SinusShooting,
			EnemyType::Wachlarz,
			EnemyType::SinusShooting,
			EnemyType::Wachlarz,
			EnemyType::Shooting,
			EnemyType::Wachlarz,
			EnemyType::Shooting,
	}, 3.f);

	Dynarray<SpawnWave> Waves
	{
		//Armaggedon, SpawnWave(Armaggedon,Vector(0.f,0.f,60.f)),Armaggedon,
		Simple, Simple, WaitSimple,
		SinusShooting, SinusShooting, SpawnWave(SinusShooting, 2.f), SpawnWave(SinusShooting, 2.f), /*WaitSimple,*/
		Shooting2, Shooting2, SpawnWave(Shooting2, 2.f), SpawnWave(Shooting2, 2.f), /*WaitSimple,*/
		Shooting3, SpawnWave(Shooting3,4.f),  Armaggedon,SpawnWave(Shooting35, 2.f), SpawnWave(Shooting3, 2.f), SpawnWave(Shooting35, 3.f), /*WaitSimple,*/
		Shooting4, Shooting2, SpawnWave(Shooting4, 2.f), SpawnWave(Shooting4, 2.f), /*WaitSimple,*/
		Armaggedon,
		Armaggedon, SpawnWave(Armaggedon,Vector(0.f,0.f,60.f)),Armaggedon,

	};

	int WaveIndex = -1;
	float WaveInterval = 2.f;
	float TimeSinceLastWave = 0.f;

};