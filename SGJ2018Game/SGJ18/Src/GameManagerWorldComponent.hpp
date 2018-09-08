#pragma once

#include "Common.hpp"
#include <UniqueID.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include "Memory/SafePtr.hpp"
#include <AnimKeys.hpp>

using namespace Poly;

class GameManagerWorldComponent : public ComponentBase
{
public:
	RTTI_DECLARE_COMPONENT(GameManagerWorldComponent) { NO_RTTI_PROPERTY(); }

	SafePtr<Entity> ActiveCamera;
	Dynarray<SafePtr<Entity>> GameEntities;
	size_t CurrentCameraIndex = 0;
	Dynarray<CameraComponent*> Cameras;
	OrderedMap<String, AnimKeys> CameraAnimations;
	AnimKeys CameraSwitches;
	float CameraSwitcherAnimProgress = 0;
};