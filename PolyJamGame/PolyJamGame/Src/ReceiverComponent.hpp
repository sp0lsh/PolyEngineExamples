#pragma once

#include <ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT ReceiverComponent : public ComponentBase
	{
	public:
		ReceiverComponent() = default;

		int ActiveTransmitters;
		float SignalStrength;
	};
}
