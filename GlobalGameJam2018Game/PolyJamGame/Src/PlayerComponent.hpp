#pragma once

#include <ECS/ComponentBase.hpp>

using namespace Poly;

namespace GGJGame
{
	class GAME_DLLEXPORT PlayerComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(PlayerComponent) { NO_RTTI_PROPERTY(); }
	public:
		PlayerComponent() = default;

		float TransmitterRepairSpeed = 2;
		float MaxHpPoints = 100;
		float ScorePoints = 0;
	};
}