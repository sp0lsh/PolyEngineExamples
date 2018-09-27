#pragma once

#include <ECS/Scene.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace TransmitterSystem
	{
		struct PercentData : public BaseObject<>
		{
			float TimeLeft;
			SafePtr<Entity> TextEnt;
		};
		// sets all TransmitterComponent properties.
		void GAME_DLLEXPORT Update(Scene* world);
		float GetPercent(Scene* world);
	}
}
