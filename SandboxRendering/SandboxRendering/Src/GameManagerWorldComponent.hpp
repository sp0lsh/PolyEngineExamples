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
	SafePtr<Entity> ShipRoot;
	SafePtr<Entity> ShipModel;
	// SafePtr<Entity> ShipCanon;
	ParticleComponent* ShipParticleSmoke;
	ParticleComponent* ShipParticleSmokeBurst;
	ParticleComponent* ShipParticleEngine;
	ParticleComponent* ShipParticleEngineBurst;

	SafePtr<Entity> Camera;
	SafePtr<Entity> CameraRootH;
	SafePtr<Entity> CameraRootV;
	SafePtr<Entity> KeyDirLight;
	Dynarray<SafePtr<Entity>> DirLights;
	Dynarray<PointLightComponent*> PointLights;
	Dynarray<Vector> PointLightPositions;
	Dynarray<SafePtr<Entity>> SpotLights;
	Dynarray<MeshRenderingComponent*> DebugMeshes;
	bool IsDrawingDebugMeshes = true;

	Dynarray<SafePtr<Entity>> GameEntities;

	ParticleComponent* particleDefault;
	ParticleComponent* particleHeart;
	ParticleComponent* particleHeartImpact0;
	ParticleComponent* particleHeartImpact1;
	ParticleComponent* particleHeartImpact2;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;

	
	// Movement
	const Vector& GetVelocity() const { return Velocity; };
	void SetVelocity(Vector& value) { Velocity = value; };
	const Vector& GetAccel() const { return Accel; };
	void SetAccel(Vector& value) { Accel = value; };
//	float GetMoveSpeed() const { return MoveSpeed; }
//	void SetMoveSpeed(float value) { MoveSpeed = value; }
	float GetFriction() const { return Friction; }
	float GetAngularVelocity() const { return RotationSpeed; }
	
	// Camera
	const float GetAngleV() const { return AngleV; }
	const float GetAngleH() const { return AngleH; }
	void SetAngleV(float value) { AngleV = value; }
	void SetAngleH(float value) { AngleH = value; }

private:
	Vector MoveForce = Vector(0.0f, 0.0f, 0.0f);
	Vector Velocity = Vector(0.0f, 0.0f, 0.0f);
	Vector Accel = Vector(0.0f, 0.0f, 0.0f);
	float Friction = 0.5f;

	// float MoveSpeed = 1.0f;
	float Acceleration = 1.0f;
	float RotationSpeed = 0.5f;

	float AngleV = 0.0f;
	float AngleH = 0.0f;
};