#pragma once

#include "ECS/ComponentBase.hpp"
#include "Rendering/Camera/CameraSystem.hpp"

namespace GGJGame {

	class GunComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(GunComponent) { NO_RTTI_PROPERTY(); }
	public:
	};
}