#pragma once

#include <ComponentBase.hpp>

namespace SGJ
{
	class GAME_DLLEXPORT GameDebugWorldComponent : public Poly::ComponentBase
	{
	public:
		Poly::SafePtr<Poly::Entity> TextID;
	};
}