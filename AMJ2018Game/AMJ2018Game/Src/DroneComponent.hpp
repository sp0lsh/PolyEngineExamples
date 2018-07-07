#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class GAME_DLLEXPORT DroneComponent : public ComponentBase
{
public:
	DroneComponent() {};

	float random;

	Vector RestPostion;
	Vector InitPosition;
	Vector InitScale;
	Quaternion InitRotation;

	PointLightComponent* Light;
	MeshRenderingComponent* LOD0;
	MeshRenderingComponent* LOD1;
	MeshRenderingComponent* LOD2;
	float LOD1threshold;
	float LOD2threshold;
	float LastDistToCamera;
};
