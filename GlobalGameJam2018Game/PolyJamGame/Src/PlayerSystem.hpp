#pragma once

#include <ECS/Scene.hpp>

using namespace Poly;

namespace GGJGame
{
	namespace PlayerSystem
	{
		void GAME_DLLEXPORT Update(Scene* world);

		void SpawnMuzzleFlash(Poly::Scene* world, Poly::EntityTransform & gunTransform, const Poly::Vector &cameraGlobFwd);

		Entity* GetPlayerCamera(Entity* player);
		Entity* GetPlayerGun(Entity* player);
		Entity* GetPlayerSoundEmmiter(Entity* player);
	}
}