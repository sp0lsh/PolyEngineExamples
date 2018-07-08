#pragma once

#include <DroneComponent.hpp>
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

	struct AnimShape : public BaseObject<>
	{
		AnimShape()
		{}

		AnimShape(Dynarray<Vector>& positions, float random)
			: Positions(positions), Random(random), ShapeSize(positions.GetSize())
		{
		}

		Dynarray<Vector> Positions;
		float Random;
		size_t ShapeSize;
	};
}


class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase
{
public:
	
	SafePtr<Entity> Camera;
	PostprocessSettingsComponent* PostCmp;
	SafePtr<Entity> Level;
	SafePtr<Entity> TextFPS;
	
	Dynarray<DroneComponent*> Drones;
	Dynarray<SafePtr<Entity>> GameEntities;

	// shapes
	Matrix ShapeTransform;
	AMJ::AnimShape AnimShape_cube;
	AMJ::AnimShape AnimShape_torus;
	AMJ::AnimShape AnimShape_logo_am;
	AMJ::AnimShape AnimShape_logo_dc;
	AMJ::AnimShape AnimShape_logo_jjb;
	AMJ::AnimShape AnimShape_logo_lg;
	AMJ::AnimShape AnimShape_logo_pn;
	AMJ::AnimShape AnimShape_logo_tl;
	AMJ::AnimShape AnimShape_logo_vm;
	Dynarray<AMJ::AnimShape> AnimShapes;

	// keys
	AMJ::AnimKeys AnimKeys;
	SafePtr<Entity> AminModel;
	float AnimProgress = 0;

public:
	bool IsDrawingDebugMeshes = true;

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