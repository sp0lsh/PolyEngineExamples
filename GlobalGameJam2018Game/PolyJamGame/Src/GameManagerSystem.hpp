#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <Utils/EnumUtils.hpp>
#include "TerrainComponent.hpp"
#include <Configs/ConfigBase.hpp>
#include <Rendering/MeshRenderingComponent.hpp>

enum class eLevels
{
	MOVEMENT_TEST = 0,
	RELEASE_LEVEL,
	_COUNT
};
namespace GGJGame
{
	namespace GameManagerSystem
	{
		void InitializeScene(Poly::Scene* world);
		void DenitializeScene(Poly::Scene* world);
		void Update(Poly::Scene* world);

		Poly::Entity* SpawnPlayer(Poly::Scene* world, const Poly::Vector& position);
		void DespawnEnemy(Poly::Scene* world);
		void DespawnPlayer(Poly::Scene* world);
		void Restart(Poly::Scene* world);

		Poly::Entity* SpawnLevel(Poly::Scene* world, eLevels level);
		void DespawnLevel(Poly::Scene* world);

		void PlaySample(Poly::Scene* world, const Poly::String& file, const Poly::Vector& position, float pitch = 1.0f, float gain = 1.0f, bool loop = false);

		void PrepareNonlevelObjects(Poly::Scene* world);
		void Cleanup(Poly::Scene* world);

		// levels spawn

		Poly::Entity* SpawnMovementTestLevel(Poly::Scene* world);
		Poly::Entity* SpawnReleaseLevel(Poly::Scene* world);

		// objects spawn

		Poly::Entity* SpawnPlaneGround(Poly::Scene* world, Poly::Entity* parent, 
			const String& meshPath = "Models/Ground_200x200.fbx", 
			eResourceSource meshResource = eResourceSource::GAME,
			Material material = Material(),
			const Vector& colliderHalfExtents = Vector(400, 400, 0),
			eTerrainPassability passability = eTerrainPassability::PASSABLE);

		Poly::Entity* SpawnLevelMesh(Poly::Scene* world, Poly::Entity* parent,
			const String& meshPath = "Models/Ground_200x200.fbx",
			eResourceSource meshResource = eResourceSource::GAME,
			Material material = Material());

		Poly::Entity* SpawnCustomCollider(Poly::Scene* world, Poly::Entity* parent,
			const String& meshPath = "Models/Ground_200x200.fbx",
			eResourceSource meshResource = eResourceSource::GAME,
			eTerrainPassability passability = eTerrainPassability::PASSABLE);		

		//enemies spawn
		void SpawnEnemies(Poly::Scene* world, Poly::Entity* parent);
		template <typename T>
		Poly::Entity* SpawnEnemy(Poly::Scene* world, Poly::Entity* parent, const Poly::Vector& position);
		Poly::Entity* SpawnBeacon(Poly::Scene* world, Poly::Entity* parent, const Poly::Vector& position);


		void SpawnAmbientFX(Scene* world);
		void SpawnParticleAmbientSmall(Scene* world);
		void SpawnParticleAmbientBig(Scene* world);

		float Random();
		float Random(float min, float max);
		Vector RandomVector(float min, float max);
	}
}
