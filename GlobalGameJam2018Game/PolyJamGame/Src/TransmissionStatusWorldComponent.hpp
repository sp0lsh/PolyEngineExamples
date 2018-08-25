#pragma once

#include "ECS/ComponentBase.hpp"
#include <ECS/Scene.hpp>
#include "Time/TimeSystem.hpp"
#include "TransmitterSystem.hpp"

namespace GGJGame
{
	class  TransmissionStatusWorldComponent : public ComponentBase
	{
		RTTI_DECLARE_COMPONENT(TransmissionStatusWorldComponent) { NO_RTTI_PROPERTY(); }
		friend void TransmitterSystem::Update(Poly::Scene* world);
		friend float TransmitterSystem::GetPercent(Scene* world);
	private:
		TransmitterSystem::PercentData PercentData;
	};
}