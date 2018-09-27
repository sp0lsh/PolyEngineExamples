#pragma once

#include <ECS/Scene.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace CollisionSystem
	{
		void GAME_DLLEXPORT Update(Scene* world);
	}
}

void Kaboom(Poly::Scene* &world);
