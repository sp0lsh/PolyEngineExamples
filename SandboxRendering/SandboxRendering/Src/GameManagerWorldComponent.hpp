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
	SafePtr<Entity> Camera;
	SafePtr<Entity> KeyDirLight;
	Dynarray<SafePtr<Entity>> DirLights;
	Dynarray<PointLightComponent*> PointLights;
	Dynarray<Vector> PointLightPositions;
	Dynarray<SafePtr<Entity>> SpotLights;
	Dynarray<MeshRenderingComponent*> DebugMeshes;
	bool IsDrawingDebugMeshes = true;

	Dynarray<SafePtr<Entity>> GameEntities;

	ParticleComponent* particleDefault;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;
	ParticleComponent* particleAmbient;
	ParticleComponent* particleAmbientWind;

	ParticleComponent* particleHeart;
	ParticleComponent* particleHeartImpact0;
	ParticleComponent* particleHeartImpact1;
	ParticleComponent* particleHeartImpact2;
};
