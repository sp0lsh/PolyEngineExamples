#pragma once

#include <LightSourceComponent.hpp>
#include <MeshRenderingComponent.hpp>
#include <ComponentBase.hpp>
#include <Dynarray.hpp>
#include <UniqueID.hpp>

using namespace Poly;

class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase
{
public:
	SafePtr<Entity> Camera;
	bool IsDrawingDebugMeshes = true;
	Dynarray<SafePtr<Entity>> GameEntities;

	Dynarray<Vector> LightsStartPositions;
	Dynarray<Entity*> PointLightEntities;
};
