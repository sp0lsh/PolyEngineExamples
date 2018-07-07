#pragma once

#include <UniqueID.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include <Rendering/MeshRenderingComponent.hpp>
#include <Rendering/PostprocessSettingsComponent.hpp>
#include <Rendering/Lighting/LightSourceComponent.hpp>

using namespace Poly;

namespace AMJ
{
	struct AnimKeys : public BaseObject<>
	{
		AnimKeys()
		{}

		AnimKeys(Dynarray<Vector>& positions, Dynarray<Quaternion>& rotations, Dynarray<Vector>& scales)
			: Positions(positions), Rotations(rotations), Scales(scales)
		{
			ASSERTE(Positions.GetSize() == Rotations.GetSize() && Positions.GetSize() == Scales.GetSize(), "size of key arrays missmatch!");
		}

		Dynarray<Vector> Positions;
		Dynarray<Quaternion> Rotations;
		Dynarray<Vector> Scales;
	};
}


class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase
{
public:
	SafePtr<Entity> Camera;
	PostprocessSettingsComponent* PostCmp;
	bool IsDrawingDebugMeshes = true;
	Dynarray<SafePtr<Entity>> GameEntities;

	AMJ::AnimKeys AnimKeys;
	SafePtr<Entity> AminModel;
	float AnimProgress = 0;

	ParticleComponent* particleDefault;
	ParticleComponent* particleAmbient;
	ParticleComponent* particleAmbientWind;
	ParticleComponent* particleLocalSpace;
	ParticleComponent* particleWorldSpace;

	ParticleComponent* particleHeart;
	ParticleComponent* particleHeartImpact0;
	ParticleComponent* particleHeartImpact1;
	ParticleComponent* particleHeartImpact2;

	Dynarray<Vector> LightsStartPositions;
	Dynarray<Entity*> PointLightEntities;
};