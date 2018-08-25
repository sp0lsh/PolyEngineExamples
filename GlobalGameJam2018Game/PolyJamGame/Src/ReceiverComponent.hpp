#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT ReceiverComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(ReceiverComponent) { NO_RTTI_PROPERTY(); }
	public:
		ReceiverComponent() = default;

		int ActiveTransmitters;
		float SignalStrength;
	};
}
