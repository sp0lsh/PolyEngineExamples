#pragma once

#include "ComponentBase.hpp"

namespace BT
{
	class GAME_DLLEXPORT StatusFlagsComponent : public Poly::ComponentBase
	{
	public:
		StatusFlagsComponent() = default;

		bool PhysicsInitialized = false;
	};

}