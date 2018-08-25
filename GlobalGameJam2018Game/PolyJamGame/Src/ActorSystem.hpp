#pragma once

#include <ECS/Scene.hpp>

using namespace Poly;

namespace GGJGame
{
	enum class eProjectileType;

	namespace ActorSystem
	{
		void GAME_DLLEXPORT Update(Scene* world);
		
		void Jump(Poly::Entity* actor);
		void Shoot(Poly::Scene* world, Poly::Entity* actor, const Vector& localOffset, const Vector& direction);
		void Move(Poly::Entity* actor, Vector direction, float speedMult = 1.0f);
		void GiveDamage(Poly::Entity* actor, float damage);
	}
}