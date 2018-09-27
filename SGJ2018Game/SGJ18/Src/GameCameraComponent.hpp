#pragma once

#include <UniqueID.hpp>
#include <Collections/Dynarray.hpp>
#include <ECS/ComponentBase.hpp>
#include "Memory/SafePtr.hpp"
#include <AnimKeys.hpp>

using namespace Poly;

enum class eGameCameraPolicy
{
	FREE,
	ORBIT,
	LOOK_AT,
	FOLLOW,
	_COUNT
};

class GameCameraComponent : public ComponentBase
{
public:
	RTTI_DECLARE_COMPONENT(GameCameraComponent) { NO_RTTI_PROPERTY(); }

	GameCameraComponent(eGameCameraPolicy policy, Entity* target = nullptr, float distance = 20.f) 
		: Policy(policy), Target(target), TargetDistance(distance) {}

	eGameCameraPolicy Policy = eGameCameraPolicy::FREE;
	float TargetDistance;
	SafePtr<Entity> Target;
};