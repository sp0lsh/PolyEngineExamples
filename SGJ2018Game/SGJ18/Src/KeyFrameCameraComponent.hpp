#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

class KeyFrameCameraComponent : public ComponentBase
{
public:
	RTTI_DECLARE_COMPONENT(KeyFrameCameraComponent) { NO_RTTI_PROPERTY(); }
	KeyFrameCameraComponent(String name) : Name(name)
	{};

	String Name;
	size_t PrevKeyFrame;
	float AnimProgress = 0.0f;
};