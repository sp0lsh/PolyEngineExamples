#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT TransmitterComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(TransmitterComponent) { NO_RTTI_PROPERTY(); }
	public:
		TransmitterComponent() = default;

		bool Transmitting = false;
		float TurnTime = 0.0f;
	};
}