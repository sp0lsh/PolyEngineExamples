#pragma once

#include <Defines.hpp>
#include <ECS/Entity.hpp>
#include <ECS/ComponentBase.hpp>
#include <Memory/SafePtr.hpp>

namespace SGJ
{
	class GameDebugWorldComponent : public Poly::ComponentBase
	{
		RTTI_DECLARE_COMPONENT(GameDebugWorldComponent) { NO_RTTI_PROPERTY(); }
	public:
		Poly::SafePtr<Poly::Entity> TextID;
	};
}