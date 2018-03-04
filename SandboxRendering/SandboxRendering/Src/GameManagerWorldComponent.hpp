#pragma once

#include <LightSourceComponent.hpp>
#include <MeshRenderingComponent.hpp>
#include <ComponentBase.hpp>
#include <ParticleComponent.hpp>
#include <Dynarray.hpp>
#include <UniqueID.hpp>

using namespace Poly;

class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase
{
public:

	SafePtr<Entity> PlayerShipRoot;
	SafePtr<Entity> PlayerShipCollision;
	ParticleComponent* ShipParticleSmoke;
	ParticleComponent* ShipParticleSmokeBurst;

	Dynarray<SafePtr<Entity>> PlayerTorpedos;
	Dynarray<float> PlayerTorpedosSpawnTime;

	Dynarray<SafePtr<Entity>> EnemyShipRoot;
	Dynarray<SafePtr<Entity>> EnemyShipCollision;
	Dynarray<ParticleComponent*> EnemyShipParticleSmokeBurst;
	Dynarray<float> EnemyVelocity;
	Dynarray<float> EnemyAngleY;
	// Dynarray<int> EnemyAlive;
	
	Dynarray<SafePtr<Entity>> EnemyTorpedos;

	SafePtr<Entity> Camera;
	SafePtr<Entity> CameraRootH;
	SafePtr<Entity> CameraRootV;
	Dynarray<SafePtr<Entity>> DirLights;
	Dynarray<PointLightComponent*> PointLights;
	Dynarray<Vector> PointLightPositions;
	Dynarray<SafePtr<Entity>> SpotLights;
	Dynarray<MeshRenderingComponent*> DebugMeshes;
	bool IsDrawingDebugMeshes = true;

	Dynarray<SafePtr<Entity>> GameEntities;
	Dynarray<SafePtr<Entity>> BombEntities;

	ParticleComponent* particleDefault;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;
	
	// Game 
	bool GetIsPaused() const { return IsPaused; }
	void SetIsPaused(bool value) { IsPaused = value; }
	bool GetNeedRestart() const { return NeedRestart; }
	void SetNeedRestart(bool value) { NeedRestart = value; }
	int GetEnemiesCount() const { return EnemiesCount; }
	void SetEnemiesCount(int value) { EnemiesCount = value; }

	// Movement
	const float GetShipAngleY() const { return ShipAngleY; }
	void SetShipAngleY(float value) { ShipAngleY = value; }

	const float GetShipVelocity() const { return ShipVelocity; }
	void SetShipVelocity(float value) { ShipVelocity = value; }

	// Camera
	const float GetCamAngleV() const { return CamAngleV; }
	const float GetCamAngleH() const { return CamAngleH; }
	void SetCamAngleV(float value) { CamAngleV = value; }
	void SetCamAngleH(float value) { CamAngleH = value; }
	
	const float GetAngularVelocity() const { return AngularVelocity; }

private:

	// Game
	bool IsPaused = true;
	bool NeedRestart = false;
	int EnemiesCount = 0;

	// Ship
	float ShipAngleY = 0.0f; // rad
	float ShipVelocity = 1.0f;

	// Camera
	float CamAngleV = 0.0f;
	float CamAngleH = 0.0f;
	float AngularVelocity = 1.0f;
};