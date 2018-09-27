#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

enum class eAnimationKeyType
{
	POSITION,
	ROTATION,
	SCALE,
	_COUNT
};

class KeyFrameAnimationComponent : public ComponentBase
{
public:
	RTTI_DECLARE_COMPONENT(KeyFrameAnimationComponent) { NO_RTTI_PROPERTY(); }

	KeyFrameAnimationComponent(String name) : Name(name)
	{};

	EnumArray<size_t, eAnimationKeyType> PrevKeyFrame = { { eAnimationKeyType::POSITION, 0 },{ eAnimationKeyType::SCALE, 0 },{ eAnimationKeyType::ROTATION, 0 } };
	EnumArray<float, eAnimationKeyType> AnimProgress = { {eAnimationKeyType::POSITION, 0.0f},{ eAnimationKeyType::SCALE, 0.0f },{ eAnimationKeyType::ROTATION, 0.0f } };
	String Name;
};