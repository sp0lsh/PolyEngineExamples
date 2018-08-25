#pragma once

#include <ECS/Scene.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace EnemyAISystem
	{
		void GAME_DLLEXPORT Update(Scene* world);
		void DeathParticle(Poly::Entity * ParticlesEnt, Poly::Scene* &world);
	}
}
