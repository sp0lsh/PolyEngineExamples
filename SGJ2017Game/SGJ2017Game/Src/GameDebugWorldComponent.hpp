#pragma once

#include <ECS/ComponentBase.hpp>
#include <Memory/SafePtr.hpp>

namespace SGJ
{
	class GAME_DLLEXPORT GameDebugWorldComponent : public Poly::ComponentBase
	{
		RTTI_DECLARE_COMPONENT(GameDebugWorldComponent) { NO_RTTI_PROPERTY(); }
	public:
		Poly::SafePtr<Poly::Entity> TextID;
	};
}