#pragma once

#include "ComponentBase.hpp"
#include <World.hpp>
#include "TimeSystem.hpp"
#include "TransmitterSystem.hpp"

namespace GGJGame
{
	class  TransmissionStatusWorldComponent : public ComponentBase
	{
		friend void TransmitterSystem::Update(Poly::World* world);
		friend int TransmitterSystem::GetPercent(World* world);
	private:
		TransmitterSystem::PercentData PercentData;
	};
}