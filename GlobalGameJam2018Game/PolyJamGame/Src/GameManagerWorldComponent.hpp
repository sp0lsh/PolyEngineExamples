#pragma once

#include <World.hpp>
#include <String.hpp>
#include <Vector.hpp>
#include <PathfindingComponent.hpp>

using namespace Poly;

class GAME_DLLEXPORT GameManagerWorldComponent : public ComponentBase {
public:
	SafePtr<Entity> Player;
	Entity* Level;
		// TODO: make this safe ptr
	MeshResource* BulletMesh;
	MeshResource* EnemyMesh;

	Poly::Dynarray<Poly::SafePtr<Poly::Entity>> SoundSampleEntities;

	bool IsWon = false;
};