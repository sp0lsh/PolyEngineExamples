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

	PointLightComponent* light;
	MeshRenderingComponent* mesh;
};