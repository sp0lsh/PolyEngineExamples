#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>

using namespace Poly;

namespace GameManagerSystem
{
	void CreateScene(World* world);
	void SpawnShaderball(World* world);
	void SpawnSponzaScene(World* world);
	void Update(World* world);
	void Deinit(World* world);

	void SpawnEmitterDefault(World* world, Vector pos);
	void SpawnEmitterAmbientDust(World* world, Vector pos);
	void SpawnEmitter1(World* world, Vector pos);
	void SpawnEmitter2(World* world, Vector pos);
	void SpawnEmitter3(World* world, Vector pos);

	void SpawnSpritesheet11(World* world, Vector pos);
	void SpawnSpritesheet22(World* world, Vector pos);
	void SpawnSpritesheet44(World* world, Vector pos);
	void SpawnSpritesheet42(World* world, Vector pos);
	void SpawnSpritesheet41(World* world, Vector pos);
	void SpawnSpritesheet44Random(World* world, Vector pos);
	void SpawnSpritesheetGandalf(World* world, Vector pos);

	void AddPointLights(World* world, int Quata);
	void CreatePointLight(World* world, float Range);
	void CreateSpotLight(World * world, float Range);
}
