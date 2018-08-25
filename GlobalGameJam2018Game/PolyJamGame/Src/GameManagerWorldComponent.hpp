#pragma once

#include <ECS/Scene.hpp>
#include <Collections/String.hpp>
#include <Math/Vector.hpp>
#include <AI/PathfindingComponent.hpp>

using namespace Poly;

class GameManagerWorldComponent : public ComponentBase {
	RTTI_DECLARE_COMPONENT(GameManagerWorldComponent) { NO_RTTI_PROPERTY(); }
public:
	SafePtr<Entity> Player;
	Entity* Level;
		// TODO: make this safe ptr
	MeshResource* BulletMesh;
	MeshResource* EnemyMesh;

	Poly::Dynarray<Poly::SafePtr<Poly::Entity>> SoundSampleEntities;

	bool IsWon = false;
};